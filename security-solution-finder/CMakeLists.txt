set(LLVM_LINK_COMPONENTS support)

add_subdirectory(criteria/find-apicalls)
add_subdirectory(criteria/find-keywords)
add_subdirectory(main-analysis)
add_subdirectory(security-patterns)

aux_source_directory(criteria/find-apicalls/ FIND-APICALLS)
aux_source_directory(criteria/find-keywords/ FIND-KEYWORDS)
aux_source_directory(main-analysis/ MAIN-ANALYSIS)
aux_source_directory(security-patterns SECURITY-PATTERNS)

add_clang_executable(security-solution-finder
  security-solution-finder.cpp
  ${MAIN-ANALYSIS}
  ${FIND-APICALLS}
  ${FIND-KEYWORDS}
  ${SECURITY-PATTERNS}
  )
target_link_libraries(security-solution-finder
  clangTooling
  clangBasic
  clangASTMatchers
  )

