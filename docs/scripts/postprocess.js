import { readdirSync, readFileSync, writeFileSync, mkdirSync, cpSync } from 'fs';
import { join, basename, dirname } from 'path';
import { fileURLToPath } from 'url';

const __dirname = dirname(fileURLToPath(import.meta.url));

const inputDir = join(__dirname, '../generated');
const outputBase = join(__dirname, '../site/src/content/docs/api');
const contentDir = join(__dirname, '../content');
const contentOutDir = join(__dirname, '../site/src/content/docs');

const homepage = `---
title: Vextr
description: A modern C++ TUI library
---
`;
mkdirSync(contentOutDir, { recursive: true });
writeFileSync(join(contentOutDir, 'index.mdx'), homepage);

if (readdirSync(contentDir).length > 0) {
    cpSync(contentDir, contentOutDir, { recursive: true });
}

function getSubdir(filename) {
    const name = basename(filename, '.md').replace(/^api-vextr--?/, '');
    const parts = name.split('--');
    if (parts.length >= 2) return parts[0];
    return '';
}

function getTitle(filename) {
    const name = basename(filename, '.md');
    const parts = name.split('--');
    const last = parts[parts.length - 1];
    return last.replace(/_/g, ' ').replace(/\b\w/g, c => c.toUpperCase());
}

for (const file of readdirSync(inputDir)) {
    if (!file.endsWith('.md')) continue;

    const raw = readFileSync(join(inputDir, file), 'utf8');
    const subdir = getSubdir(file);
    const outDir = subdir ? join(outputBase, subdir) : outputBase;
    mkdirSync(outDir, { recursive: true });

    const title = getTitle(file);
    let content = raw.startsWith('---') ? raw : `---\ntitle: ${title}\n---\n\n${raw}`;

    content = content.replace(/\n\{#[a-zA-Z0-9_-]+\}\n/g, '\n');
    content = content.replace(/\{#[a-zA-Z0-9_-]+\}/g, '');

    const baseName = file.replace(/^api-vextr--?/, '').replace(/\.md$/, '');
    const lastPart = baseName.split('--').pop().toLowerCase();
    const currentPath = subdir ? `/api/${subdir}/${lastPart}` : `/api/${lastPart}`;

    content = content.replace(/\]\(([^)]*?)(\.md)?(#[a-zA-Z0-9_-]*)?\)/g, (match, path, ext, anchor) => {
        if (!path) {
            return `](${currentPath}${anchor || ''})`;
        }

        let cleanPath = path
            .replace(/^api-vextr--?/, '')
            .replace(/--/g, '/')
            .toLowerCase();
        const href = `/api/${cleanPath}${anchor || ''}`;
        return `](${href})`;
    });

    const outFileName = lastPart + '.mdx';

    writeFileSync(join(outDir, outFileName), content);
}

console.log('Done.');