
# 递归查找所有源文件，并分组到目录中  
function(group_sources curdir basedir)  
    file(GLOB children RELATIVE "${curdir}" "${curdir}/*")  
    foreach(child ${children})  
        if(IS_DIRECTORY ${curdir}/${child})  
            # 递归调用 group_sources 函数  
            group_sources("${curdir}/${child}" "${basedir}")  
        elseif(child MATCHES ".*\\.(cpp|h|hpp|c|cxx|ino)$")  
            # 将源文件添加到相应的组中  
            string(REPLACE "/" "\\" group_path ${curdir}) 
			string(REPLACE "/" "\\" newBaseDir ${basedir}) 
            string(REPLACE "${newBaseDir}\\" "" group_path ${group_path})  
            source_group("${group_path}" FILES "${curdir}/${child}")  
        endif()  
    endforeach()  
endfunction()
