#!/bin/bash

put() {
	cat "$1" | grep -v "#pragma once" | grep -v "#include"
}

echo "#pragma once"
put "src/test.h"

echo -e "\n#ifdef _CTEST_IMPL"
put "src/ctest_impl.h"
find src -type f | xargs cat | grep "#include" | grep -v '"test.h"' | grep -v '"ctest_impl.h"' | sort | uniq

for file in $(ls src); do
	if [[ "$file" != "test.h" && "$file" != "ctest_impl.h" ]]; then
		echo -e "\n/*\n * $file\n */\n"
		put "src/$file"
	fi
done

echo -e "\n#endif // _CTEST_IMPL"
