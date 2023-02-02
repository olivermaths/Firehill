



struct Vector2D<T: FloatingPoint>{
    var x, y : T
    init(x: T, y: T){
        (self.x, self.y) = (x, y)
    }
}

struct Vector3D<T: FloatingPoint>{
    var x, y, z: T
    init(x: T, y: T, z: T){
        (self.x, self.y, self.z) = (x, y, z)
    }
}


infix operator .*: MultiplicationPrecedence 
extension Vector2D{
    static func + (left: Vector2D, right: Vector2D) -> Vector2D {
        return Vector2D(x: left.x + right.x, y: left.y + right.y)
    }

    static func - (left: Vector2D, right: Vector2D) -> Vector2D {
        return Vector2D(x: left.x - right.x, y: left.y - right.y)
    }
    
    static func .* (left: Vector2D, right: Vector2D) -> T {
        return (left.x * right.x) + (left.y + right.y)
    }

    static func * (left: T, right: Vector2D) -> Vector2D {
        Vector2D(x: right.x * left, y: right.y * left)
    }

    static func * (left: Vector2D, right: Vector2D) -> Vector2D {
        Vector2D(x: right.x * left.x, y: right.y * left.y)
    }


    static func += (left: inout Vector2D, right: Vector2D){
        left = left + right
    }

    static func *= (left: inout Vector2D, right: Vector2D){
        left = left * right
    }

    static func -= (left: inout Vector2D, right: Vector2D){
        left = left - right
    }

}