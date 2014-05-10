#!/bin/bash

shopt -s globstar

text="////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////"

types=( 
    ".cpp" 
    ".hpp" 
)

folders=( 
    "../include" 
    "../src" 
    "../examples" 
)

for folder in "${folders[@]}"
do
    for type in "${types[@]}"
    do
        for file in ${folder}/**/*${type}
        do
            if [ -f $file ]
            then
                if grep -q "Swift2D" ${file}
                then
                    echo "Reformatting ${file} ..."
                    sed -e '1,7d' ${file} > /tmp/copyright_tmp && mv /tmp/copyright_tmp ${file}
                    echo "${text}" > /tmp/copyright_tmp
                    cat ${file} >> /tmp/copyright_tmp && mv /tmp/copyright_tmp ${file}
                fi
            fi
        done
    done
done



