package fmj.characters

enum class Direction(val v: Int) {
    North(1),
    East(2),
    South(3),
    West(4);
    companion object {
        fun fromInt(v: Int): Direction {
            return when (v) {
                1 -> Direction.North
                2 -> Direction.East
                3 -> Direction.South
                4 -> Direction.West
                else -> {
                    println("Direction.fromInt invalid v=$v")
                    Direction.North
                }
            }
        }
    }
}
