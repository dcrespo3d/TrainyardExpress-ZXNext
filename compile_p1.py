f = open('page1.asm', 'rt')
txt = f.read()
f.close()

prolog = 'SECTION PAGE_38_TUTORIALS\nORG $0000\n'
txt = prolog + txt

txt = txt.replace('code_compiler', 'PAGE_38_TUTORIALS')
txt = txt.replace('rodata_compiler', 'PAGE_38_TUTORIALS')
txt = txt.replace('data_compiler', 'PAGE_38_TUTORIALS')

f = open('page1.asm', 'wt')
f.write(txt)
f.close()