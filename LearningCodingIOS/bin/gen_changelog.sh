#!/bin/bash

echo "Branch: $REFSPEC" > changelog.txt
echo "" >> changelog.txt
$(git log --pretty=format:"* %h %an: %s" --no-merges | head -n 15 >> changelog.txt)
echo "" >> changelog.txt
