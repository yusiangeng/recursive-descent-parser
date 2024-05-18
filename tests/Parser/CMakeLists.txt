add_executable(
  run-parser-tests
  run-tests.cpp
  assignment-test.cpp
  block-test.cpp
  do-while-test.cpp
  empty-statement-test.cpp
  equality-test.cpp
  for-test.cpp
  if-test.cpp
  literals-test.cpp
  logical-test.cpp
  math-test.cpp
  relational-test.cpp
  statement-list-test.cpp
  unary-test.cpp
  variable-test.cpp
  while-test.cpp)

target_link_libraries(run-parser-tests Parser)
