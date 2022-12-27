f = open('sound_freqs.csv', 'rt')
itxt = f.read()
f.close()

otxt = '''// auto-generated by python script sound_freqs.py
#ifndef __sound_freqs_py__
#define __sound_freqs_py__

// AY-3-8192 periods for musical notes
//
// period = 125000 / freq
'''
endl = '\n'

def rpad3(s):
    while len(s) < 3:
        s = s + ' '
    return s

def lpad4(s):
    while len(s) < 4:
        s = ' ' + s
    return s

def lpad7(s):
    while len(s) < 7:
        s = ' ' + s
    return s

for iline in itxt.splitlines():
    if not iline.startswith(';'): continue
    toks = iline.split(';')
    a,b,c,d,e,f = toks
    oline = '#define T_' + rpad3(b + c) + ' ' + lpad4(e) + '    // f = ' + lpad7(d) + endl
    otxt += oline
    # print(oline)

otxt += endl
otxt += '#endif  // __sound_freqs_py__' + endl

print(otxt)

f = open('sound_freqs.h', 'wt')
f.write(otxt)
f.close()