// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Firehill",
    products: [
        // Products define the executables and libraries a package produces, and make them visible to other packages.
        .executable(name: "Sandbox", targets: ["Sandbox"]),
        .library(
            name: "Firehill",
            targets: ["Firehill"]),
    ],
    dependencies: [
        // Dependencies declare other packages that this package depends on.
        // .package(url: /* package url */, from: "1.0.0"),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .executableTarget(
            name: "Sandbox",
            dependencies: ["Firehill"]
            ),
        .target(
            name: "Firehill",
            dependencies: ["CFirehill"]),        
        .target(
            name: "CFirehill",
            dependencies: [],
            linkerSettings: [
                .linkedLibrary("Libs/vulkan"),
                .linkedLibrary("Libs/glfw3")
            ]
            ),
        .testTarget(
            name: "FirehillTests",
            dependencies: ["Firehill"]),
    ]
)
