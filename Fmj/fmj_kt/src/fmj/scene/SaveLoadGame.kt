package fmj.scene

import fmj.characters.NPC
import fmj.characters.Player
import fmj.characters.SceneObj
import fmj.characters.WalkingSprite
import fmj.combat.Combat
import fmj.lib.DatLib
import fmj.script.ScriptProcess
import fmj.views.Game
import fmj.DebugLogger
import fmj.config.GameSettings
import graphics.Point
import java.ObjectInput
import java.ObjectOutput
import java.readArray
import java.writeArray
import kotlin.sequences.sequence

object SaveLoadGame {
    const val magicNum = 0x67736176
    const val version = 6

    /**
     * 是否开始新游戏
     */
    var startNewGame = true

    /**
     * 当前地图编号
     */
    var MapType = 1
    var MapIndex = 1

    /**
     * 屏幕左上角在地图中的位置
     */
    var MapScreenX = 1
    var MapScreenY = 1

    /**
     * 当前脚本编号
     */
    var ScriptType = 1
    var ScriptIndex = 1

    /**
     * 场景名称
     */
    var SceneName = ""

    var NpcObjs: Array<NPC> = arrayOf()
    var scriptProcess: ScriptProcess? = null

    var playerDb: MutableList<Player> = arrayListOf()

    var allowTossArm = true

    fun loadPlayers() {
        playerDb = sequence {
            (0..25).forEach {
                yield(DatLib.getRes(DatLib.ResType.ARS, 1, it, true) as Player?)
            }
        }.filterNotNull().toMutableList()
    }

    fun getPlayerByIndex(index: Int): Player? {
        for (p in playerDb) {
            if (p.index == index)
                return p
        }
        return null
    }

    fun write(game: Game, out: ObjectOutput) {
        DebugLogger.SaveLoad.saveStarted(-1, SceneName)
        
        out.writeString(SceneName)
        val actorNum = game.playerList.size
        out.writeInt(actorNum)
        for (i in 0 until actorNum) {
            out.writeInt(game.playerList[i].index)
        }
        out.writeInt(magicNum)
        out.writeInt(version)
        out.writeInt(MapType)
        out.writeInt(MapIndex)
        
        // 🔧 检查并修正存档时的屏幕位置坐标
        val safeScreenX = if (MapScreenX < 0 || MapScreenX > 1000) {
            DebugLogger.warn(DebugLogger.Tags.SAVE_LOAD, "FixSaveScreenPos", 
                "存档时修正异常的MapScreenX: $MapScreenX -> 4")
            4
        } else MapScreenX
        
        val safeScreenY = if (MapScreenY < 0 || MapScreenY > 1000) {
            DebugLogger.warn(DebugLogger.Tags.SAVE_LOAD, "FixSaveScreenPos", 
                "存档时修正异常的MapScreenY: $MapScreenY -> 4")
            4
        } else MapScreenY
        
        out.writeInt(safeScreenX)
        out.writeInt(safeScreenY)
        out.writeInt(ScriptType)
        out.writeInt(ScriptIndex)

        // version 2
        out.writeBoolean(GameSettings.allowMiss)
        // version 2
        out.writeBoolean(allowTossArm)

        game.mainScene.scriptProcess.encode(out)

        out.writeInt(playerDb.size)
        for (i in 0 until playerDb.size) {
            playerDb[i].encode(out)
        }
        out.writeLong(Player.sMoney.toLong())
        Player.sGoodsList.write(out)
            
        // 保存前打印NPC信息
        println("===== 保存时的NPC列表 =====")
        for (i in NpcObjs.indices) {
            if (!NpcObjs[i].isEmpty) {
                val npc = NpcObjs[i]
                println("保存NPC[$i]: type=${npc.type}, index=${npc.index}, name='${npc.name}', " +
                        "pos=(${npc.posInMap.x},${npc.posInMap.y}), isSceneObj=${npc is SceneObj}")
            }
        }
        println("总共保存 ${NpcObjs.count { !it.isEmpty }} 个NPC")
        println("=========================")
        
        writeArray(out, NpcObjs) {
            io, obj ->

            if (obj.isEmpty) {
                io.writeByte(0)
                DebugLogger.trace(DebugLogger.Tags.SAVE_LOAD, "SaveEmpty", "保存空NPC槽位")
            } else {
                if(obj is SceneObj) {
                    io.writeByte(2)
                    println("保存SceneObj: type=${obj.type}, index=${obj.index}, name='${obj.name}'")
                } else {
                    io.writeByte(1)
                    println("保存NPC: type=${obj.type}, index=${obj.index}, name='${obj.name}'")
                }
                obj.encode(io)
            }
        }
        Combat.write(out)
    }

    fun read(game: Game, coder: ObjectInput): Boolean {
        DebugLogger.SaveLoad.loadStarted(-1)
        
        SceneName = coder.readString()
        DebugLogger.info(DebugLogger.Tags.SAVE_LOAD, "LoadOperation", 
            "开始读取存档场景: $SceneName")
            
        var actorNum = coder.readInt()
        val playerIds = sequence {
            while (actorNum-- > 0)
                yield(coder.readInt())
        }.toList()

        val m = coder.readInt()
        val version = if (m == magicNum) {
            0
        } else {
            coder.readInt()
        }
        if (version < 4) {
            DebugLogger.error(DebugLogger.Tags.SAVE_LOAD, "LoadError", 
                "不兼容的存档版本: $version")
            game.showMessage("不兼容的存档版本")
            return false
        }
        coder.version = version
        MapType = if (version == 0) {
            m
        } else {
            coder.readInt()
        }
        MapIndex = coder.readInt()
        MapScreenX = coder.readInt()
        MapScreenY = coder.readInt()
        ScriptType = coder.readInt()
        ScriptIndex = coder.readInt()
        
        // 🔧 检查并修正异常的屏幕位置坐标
        val originalScreenX = MapScreenX
        val originalScreenY = MapScreenY
        
        // 修正负数或过大的屏幕坐标
        if (MapScreenX < 0 || MapScreenX > 1000) {
            MapScreenX = 9  // 使用默认的安全位置
            DebugLogger.warn(DebugLogger.Tags.SAVE_LOAD, "FixScreenPos", 
                "修正异常的MapScreenX: $originalScreenX -> $MapScreenX")
        }
        
        if (MapScreenY < 0 || MapScreenY > 1000) {
            MapScreenY = 5  // 使用默认的安全位置
            DebugLogger.warn(DebugLogger.Tags.SAVE_LOAD, "FixScreenPos", 
                "修正异常的MapScreenY: $originalScreenY -> $MapScreenY")
        }
        
        DebugLogger.debug(DebugLogger.Tags.SAVE_LOAD, "LoadMapInfo", 
            "地图信息 - 类型: $MapType, 索引: $MapIndex, 屏幕位置: ($MapScreenX, $MapScreenY)")
            
        if (version >= 2) {
            GameSettings.allowMiss = coder.readBoolean()
            allowTossArm = coder.readBoolean()
        } else {
            GameSettings.allowMiss = false
            allowTossArm = true
        }
        scriptProcess = game.vm.loadScript(SaveLoadGame.ScriptType, SaveLoadGame.ScriptIndex)
        scriptProcess?.decode(coder)

        val size = coder.readInt()
        playerDb.clear()
        for (i in 0 until size) {
            val p = Player()
            p.decode(coder)
            playerDb.add(p)
        }

        game.playerList.clear()
        game.playerList.addAll(
                playerIds.map {
                    getPlayerByIndex(it)
                }.filterNotNull()
        )

        Player.sMoney = if (coder.version >= 5) {
            coder.readLong().toInt()
        } else {
            coder.readInt()
        }
        Player.sGoodsList.read(coder)

        // 读取NPC数据前记录
        DebugLogger.info(DebugLogger.Tags.SAVE_LOAD, "LoadNPCs", 
            "开始恢复NPC数据...")
            
        NpcObjs = readArray(coder) {
            val type = it.readByte()
            val npc =
                    when (type.toInt()) {
                        0, 1 -> NPC()
                        2 -> SceneObj()
                        else -> throw Error("Bad obj type: $type")
                    }
            if (type.toInt() != 0) {
                npc.decode(it)
            }
            npc
        }

        Combat.read(game, coder)
        return true
    }
}
