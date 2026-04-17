import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

export default defineConfig({
    integrations: [
        starlight({
            title: 'Vextr',
            social: [
                {icon: 'github', label: 'GitHub', href: 'https://github.com/Oltsu-code/Vextr'},
            ],
            sidebar: [
                { label: 'Guide', autogenerate: { directory: 'guide' }, collapsed: true },
                { label: 'API References', autogenerate: { directory: 'api' }, collapsed: true },
            ],
        }),
    ],
});