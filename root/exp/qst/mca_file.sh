#/bin/bash


pwd=${PWD}
qst_dir=/np1a/v05/qst/exp2019/ws_laptop1_backup_201911061149/HPGe

cd ${qst_dir}
find ./ -type f | xargs grep -sl '\[Header\]' | xargs grep -sl 'ch,CH1,CH2,CH3,CH4' | sed -e "s/^.\///" > ${pwd}/mca_file.txt

