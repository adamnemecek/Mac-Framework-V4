xcodebuild -create-xcframework -framework Paddle.framework -output Paddle.xcframework
# git add Paddle.xcframework
# git touch Package.swift 
# copy the contents from my Package.swift (https://github.com/adamnemecek/Mac-Framework-V4/blob/master/Package.swift, consider changing some things as you see fit). 
# git commit -am 'added SPM Support'