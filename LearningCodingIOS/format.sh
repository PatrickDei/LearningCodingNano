CLANG_FORMAT="$HOME/Library/Application Support/Alcatraz/Plug-ins/ClangFormat/bin/clang-format"

for DIRECTORY in StoryGame/Classes StoryGame/ios
do
    echo "Formatting code under $DIRECTORY/"
    find "$DIRECTORY" \( -name '*.h' -or -name '*.m' -or -name '*.cpp' -or -name '*.mm' \) -print0 | xargs -0 "$CLANG_FORMAT" -i
done