import { readdirSync, readFileSync, writeFileSync, mkdirSync, cpSync, existsSync, rmSync } from 'fs';
import { join, basename, dirname } from 'path';
import { fileURLToPath } from 'url';

const __dirname = dirname(fileURLToPath(import.meta.url));

const GENERATED_DIR = join(__dirname, '../generated');
const CONTENT_DIR   = join(__dirname, '../content');
const DOCS_OUT      = join(__dirname, '../site/src/content/docs');
const GUIDE_OUT     = join(DOCS_OUT, 'guide');
const API_OUT       = join(DOCS_OUT, 'api');

rmSync(DOCS_OUT, { recursive: true, force: true });

function titleFromFilename(filename) {
    return basename(filename).replace(/\.(md|mdx)$/, '').replace(/[-_]/g, ' ').replace(/\b\w/g, c => c.toUpperCase());
}

function titleFromDirname(dirname) {
    return basename(dirname).replace(/[-_]/g, ' ').replace(/\b\w/g, c => c.toUpperCase());
}

function extractTitleFromContent(filename, content) {
    const match = content.match(/^#+\s+(.+)$/m);
    if (match) return match[1].trim();
    return titleFromFilename(filename);
}


function stripAnchorNumbers(content) {
    return content.replace(/\(\/api\/[^)]+#([a-zA-Z0-9_-]+)\)/g, (match) => {
        return match.replace(/#([a-zA-Z0-9_-]+)-\d+\)/, '#$1)');
    });
}
function injectFrontmatter(content, title) {
    if (content.startsWith('---')) return content;
    return `---\ntitle: ${title}\n---\n\n${content}`;
}

function stripHeading(content) {
    return content.replace(/^#+\s+.+\n?/m, '');
}

function isEffectivelyEmpty(content) {
    const withoutFrontmatter = content.replace(/^---[\s\S]*?---\n?/, '');
    const stripped = withoutFrontmatter
        .replace(/^#+\s+.+$/gm, '')
        .replace(/^import\s+.+$/gm, '')
        .trim();
    return stripped.length === 0;
}

function transformAdmonitions(content) {
    content = content.replace(/@tip\s+([^\n]+)/gi, (_, line) => `:::tip\n${line.trim()}\n:::`);
    content = content.replace(/:::(warning|attention)\s*([\s\S]*?):::/gi, (_, _tag, body) => `:::caution\n${body.trim()}\n:::`);
    return content;
}

function stripAnchorTags(content) {
    content = content.replace(/\n\{#[a-zA-Z0-9_-]+\}\n/g, '\n');
    content = content.replace(/\{#[a-zA-Z0-9_-]+\}/g, '');
    return content;
}

function rewriteLinks(content, currentPath) {
    return content.replace(/\]\(([^)]*?)(\.md)?(#[a-zA-Z0-9_-]*)?\)/g, (match, path, _ext, anchor) => {
        if (!path) return `](${currentPath}${anchor || ''})`;
        const cleanPath = path.replace(/^api-vextr--?/, '').replace(/--/g, '/').toLowerCase();
        return `](${ '/api/' + cleanPath}${anchor || ''})`;
    });
}

function pagesIn(dir) {
    if (!existsSync(dir)) return [];
    return readdirSync(dir, { withFileTypes: true })
        .filter(e => e.isFile() && (e.name.endsWith('.md') || e.name.endsWith('.mdx')) && e.name !== 'index.mdx')
        .sort((a, b) => a.name.localeCompare(b.name))
        .map(e => e.name.replace(/\.(md|mdx)$/, ''));
}

function capitalize(s) {
    return s.charAt(0).toUpperCase() + s.slice(1);
}

const API_DESCRIPTIONS = {
    std:     'Standard library utilities and type aliases used throughout Vextr.',
    vextr:   'Top-level Vextr namespace — core entry points and global configuration.',
    app:     'The `App` class — initializes the terminal, owns the main event loop, and drives rendering.',
    backend: 'Low-level terminal backend: raw I/O, cell buffers, and the renderer that flushes diffs to the screen.',
    core:    'Foundational types used across the entire library — widgets, layouts, styles, events, colors, and geometry.',
    layout:  'Layout engines: `StackLayout`, `GridLayout`, and `AbsoluteLayout` for composing widget trees.',
    utils:   'Utility modules: Unicode handling, ANSI sequences, input parsing, and string helpers.',
    widgets: 'Ready-to-use widgets: `Label`, `Button`, `Input`, `Dropdown`, `Modal`, `ScrollView`, and more.',
};

function writeApiRootIndex(dir) {
    const sections = readdirSync(dir, { withFileTypes: true })
        .filter(e => e.isDirectory())
        .sort((a, b) => a.name.localeCompare(b.name))
        .map(e => e.name);

    const links = sections.map(s => `- [${capitalize(s)}](/api/${s})`).join('\n');

    writeFileSync(join(dir, 'index.mdx'),
        `---\ntitle: API Reference\n---
        
        API Reference for Vextr. Organized by module/namespace.
        
        ${links}`
    );
    console.log('API root index written');
}

const SPLASH = `---
title: Vextr
description: A modern C++ TUI library
template: splash
hero:
  tagline: A flexible widget-based terminal UI framework for modern C++.
  actions:
    - text: Get Started
      link: /guide/getting-started/introduction
      icon: right-arrow
      variant: primary
    - text: API Reference
      link: /api
      icon: open-book
---

import { Card, CardGrid } from '@astrojs/starlight/components';
`;

function processContentDir(sourceDir, targetDir) {
    if (!existsSync(sourceDir)) return;

    for (const entry of readdirSync(sourceDir, { withFileTypes: true })) {
        const src  = join(sourceDir, entry.name);
        const dest = join(targetDir, entry.name);

        if (entry.isDirectory()) {
            mkdirSync(dest, { recursive: true });
            processContentDir(src, dest);
        } else if (entry.name.endsWith('.md') || entry.name.endsWith('.mdx')) {
            const raw     = readFileSync(src, 'utf8');
            const title   = titleFromFilename(entry.name);
            const body    = stripHeading(raw);
            const processed = injectFrontmatter(body, title);
            if (isEffectivelyEmpty(processed)) {
                console.log(`Skipping empty: ${entry.name}`);
                continue;
            }
            writeFileSync(dest, processed);
        } else {
            cpSync(src, dest);
        }
    }
}

function processApiFile(file) {
    const raw    = readFileSync(join(GENERATED_DIR, file), 'utf8');
    const title  = extractTitleFromContent(file, raw);
    const name   = basename(file, '.md').replace(/^api-vextr--?/, '');
    const parts  = name.split('--');
    const subdir = parts.length >= 2 ? parts[0] : '';
    const slug   = parts[parts.length - 1].toLowerCase();
    const outDir = subdir ? join(API_OUT, subdir) : API_OUT;
    const currentPath = subdir ? `/api/${subdir}/${slug}` : `/api/${slug}`;

    let content = stripHeading(raw);
    content = injectFrontmatter(content, title);
    content = stripAnchorTags(content);
    content = transformAdmonitions(content);
    content = rewriteLinks(content, currentPath);
    content = stripAnchorNumbers(content);

    if (isEffectivelyEmpty(content)) {
        console.log(`Skipping empty: ${file}`);
        return;
    }

    mkdirSync(outDir, { recursive: true });
    writeFileSync(join(outDir, slug + '.mdx'), content);
    console.log(`API: ${currentPath}`);
}

mkdirSync(DOCS_OUT, { recursive: true });
writeFileSync(join(DOCS_OUT, 'index.mdx'), SPLASH);

processContentDir(CONTENT_DIR, GUIDE_OUT);

for (const file of readdirSync(GENERATED_DIR).filter(f => f.endsWith('.md'))) {
    processApiFile(file);
}

writeApiRootIndex(API_OUT);

console.log('Done.');
