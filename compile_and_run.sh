if [ "$1" == "java" ]; then
	(echo "Compiling...") \
	&& (javac java_src/*.java) \
	&& (echo "Compiled successfully! Running...") \
	&& (echo "") \
	&& (java -ea -classpath java_src MainDriver $2)
elif [ "$1" == "c++" ]; then
	echo "WIP..."
else
	echo "Incorrect parameters: choose 'java' or 'c++'"
fi
