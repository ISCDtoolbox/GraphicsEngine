

set(ENV{VS_UNICODE_OUTPUT} "")
set(command "/usr/bin/cmake;-P;/home/foulonj/dev/SUscCGL_V2/build/library/glm/tmp/glm-gitclone.cmake")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/foulonj/dev/SUscCGL_V2/build/library/glm/src/glm-stamp/glm-download-out.log"
  ERROR_FILE "/home/foulonj/dev/SUscCGL_V2/build/library/glm/src/glm-stamp/glm-download-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/foulonj/dev/SUscCGL_V2/build/library/glm/src/glm-stamp/glm-download-*.log\n")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "glm download command succeeded.  See also /home/foulonj/dev/SUscCGL_V2/build/library/glm/src/glm-stamp/glm-download-*.log\n")
  message(STATUS "${msg}")
endif()
