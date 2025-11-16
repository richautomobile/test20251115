#! /bin/sh
# generator git info
rm -f ./source/system/gitinfo.h
echo "#ifndef GITINFO_H" > ./source/system/gitinfo.h
echo "#define GITINFO_H" >> ./source/system/gitinfo.h
echo "#define BUILD_DATE $(date +"%Y-%m-%d_%H-%M-%S")" >> ./source/system/gitinfo.h
echo "#define GIT_DATE $(git log -1 --pretty=format:"%ad" --date=format:"%Y-%m-%d_%H-%M-%S")" >> ./source/system/gitinfo.h
echo "#define GIT_BRANCH $(git rev-parse --abbrev-ref HEAD)" >> ./source/system/gitinfo.h
echo "#define GIT_HASH $(git rev-parse HEAD)" >> ./source/system/gitinfo.h
echo "#endif" >> ./source/system/gitinfo.h
# generator lot3_project.mk for maakefile
#awk '!/\/\*/ && /#define/ {if (NF >=3) print $2 " = " $3}' ./source/system/project.h > ./temp.mk
awk '!/\/\*/ && /#define/ {
    if (NF>=3) {
        printf "%s = %s",$2, $3;
        for(i=4; i<= NF; i++){
            printf " %s",$i;
        }
        print "";
    }
}' ./source/system/project.h > ./temp.mk
tr -d '\r\t()' < ./temp.mk > ./lot3_project.mk
rm ./temp.mk
