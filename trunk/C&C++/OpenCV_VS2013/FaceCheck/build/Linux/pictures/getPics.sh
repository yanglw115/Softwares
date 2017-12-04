

#!/bin/sh

NameDatabase=USER_FACE_RECOGNITION_RESULT.txt
DirPicsOut=pics

sed -i 's/\r//g' ${NameDatabase}

# awk -F '\t' '{ if (NR > 1) print $3}' ${NameDatabase}
awk -F '\t' '{ if (NR > 1) {cmd = "wget -P pics "$3;  system(cmd)} }' ${NameDatabase}
# awk -F '\t' '{ if (NR > 1) {"ls -l pics/${3##*/} | wc -l" | getline num; print num} }' ${NameDatabase} # 尚未成功，后续优化
