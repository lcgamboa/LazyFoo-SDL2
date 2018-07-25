#!/bin/bash 

rm -Rf docs
dirs=`find * -maxdepth 0 -type d 2> /dev/null`
mkdir -p "docs"
echo "<!DOCTYPE html>" >> docs/index.html 
echo "<html><body><center>" >> docs/index.html 
echo "<br><h1>Emscripten Lazy Foo' Examples</h1><br>" >> docs/index.html 
echo "<table border=1><tr><th>Lazy Foo' tutorial</th><th>Emscripten online version</th></tr>" >> docs/index.html
for example in $dirs
do
  echo "examples: $example"	
  mkdir -p "docs/$example/"
  echo "<tr><td><br><a href=http://lazyfoo.net/tutorials/SDL/$example/index.php target=_blank>$example</a></td>" >> docs/index.html 
  echo "<td><a href="$example/$example.html" target=_blank>$example</a></td></tr>" >> docs/index.html 
  cd $example
  cp  *.js *.data *.wasm *.html *.mem ../docs/$example/
  cd ..
done
echo "</table>" >> docs/index.html 
echo "</center></body></html>" >> docs/index.html 
rm -Rf docs/docs/
