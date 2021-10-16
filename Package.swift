// swift-tools-version:5.4
import PackageDescription

let package = Package(
    name: "Paddle",
    platforms: [
        .macOS(.v10_14)
    ],
    products: [
        // Products define the executables and libraries a package produces, and make them visible to other packages.
        .library(
            name: "Paddle",
            targets: ["PaddleFramework"])
    ],
    dependencies: [
        // Dependencies declare other packages that this package depends on.
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .target(
            name: "PaddleSDK"
        ),
        .binaryTarget(
            name: "PaddleFramework",
            path: "Paddle.xcframework"
        )
        // .testTarget(
        //     name: "MyLibraryTests",
        //     dependencies: ["MyLibrary"]
            ),
    ]
)