FILE(REMOVE_RECURSE
  "CMakeFiles/glm"
  "CMakeFiles/glm-complete"
  "library/glm/src/glm-stamp/glm-install"
  "library/glm/src/glm-stamp/glm-mkdir"
  "library/glm/src/glm-stamp/glm-download"
  "library/glm/src/glm-stamp/glm-update"
  "library/glm/src/glm-stamp/glm-patch"
  "library/glm/src/glm-stamp/glm-configure"
  "library/glm/src/glm-stamp/glm-build"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/glm.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
