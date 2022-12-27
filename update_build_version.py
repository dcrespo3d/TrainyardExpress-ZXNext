f = open('update_build_version.txt', 'rt')
txt = f.read()
f.close()

vernum = int(txt)
vernum += 1

f = open('update_build_version.txt', 'wt')
f.write(str(vernum))
f.close()

f = open('update_build_version.h', 'wt')
f.write('#ifndef __update_build_version_h__\n')
f.write('#define __update_build_version_h__\n')
f.write('\n')

txt = '#define BUILD_VER "BLD '
verstr = hex(vernum).replace('0x', '').upper()
while len(verstr) < 6:
    verstr = '0' + verstr
txt += verstr
txt += '"\n'

from datetime import datetime;
now = datetime.now()
date_str = now.strftime("%Y/%m/%d!%H:%M:%S")

txt += '#define BUILD_DATE "'
txt += date_str.split('!')[0];
txt += '"\n'

txt += '#define BUILD_TIME " '
txt += date_str.split('!')[1];
txt += ' "\n'

txt += '#define BUILD_STR "BUILD ' + verstr + ' '
txt += date_str.split('!')[0] + ' '
txt += date_str.split('!')[1] + '"\n'

f.write(txt);

f.write('\n')
f.write('#endif //  __update_build_version_h__\n')
f.close()