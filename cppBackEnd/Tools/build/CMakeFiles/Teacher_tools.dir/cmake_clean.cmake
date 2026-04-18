file(REMOVE_RECURSE
  "libTeacher_tools.a"
  "libTeacher_tools.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Teacher_tools.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
