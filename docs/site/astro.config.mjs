import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

export default defineConfig({
    integrations: [
        starlight({
            title: 'Vextr',
            sidebar: [
                { label: 'Getting Started', link: '/' },
                { label: 'API', autogenerate: { directory: 'api' } },
            ],
        }),
    ],
});