#!/bin/sh

make
rm ./serverlogs/1.config
rm ./serverlogs/2.config
rm ./serverlogs/3.config
rm ./serverlogs/server.output

bin/runtest.sh 10 tests/sampletest.txt 2> /dev/null