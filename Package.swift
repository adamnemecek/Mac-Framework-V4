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
            targets: ["Paddle"])
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .binaryTarget(
            name: "Paddle",
            path: "Paddle.xcframework"
        )
    ]
)

// import PackageDescription

// let package = Package(
//     name: "Paddle",
//     products: [
//         .library(
//             name: "Paddle",
//             targets: ["Paddle"]
//         ),
//     ],
//     dependencies: [],
//     targets: [
//         .binaryTarget(
//             name: "Paddle",
//             url: "https://files.lowtechguys.com/Paddle.xcframework.zip",
//             checksum: "e4b6194ea85c268672c4ca64e742c4b8fef6071449a07ee0f5d263e080f06345"
//         ),
//     ]
// )

