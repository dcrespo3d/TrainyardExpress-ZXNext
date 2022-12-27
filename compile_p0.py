f = open('page0.asm', 'rt')
txt = f.read()
f.close()

prolog = 'SECTION PAGE_36_PAGE0\nORG $0000\n'
txt = prolog + txt

txt = txt.replace('code_compiler', 'PAGE_36_PAGE0')
txt = txt.replace('rodata_compiler', 'PAGE_36_PAGE0')
txt = txt.replace('data_compiler', 'PAGE_36_PAGE0')

f = open('page0.asm', 'wt')
f.write(txt)
f.close()