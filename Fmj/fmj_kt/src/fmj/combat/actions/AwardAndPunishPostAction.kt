package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.combat.anim.Animation
import graphics.Canvas

class AwardAndPunishPostAction(attackers: Iterable<FightingCharacter>): PostAction() {
    var animations: MutableList<Animation> = arrayListOf()
    init {
        for (attacker in attackers) {
            if (!attacker.isAlive) {
                continue
            }
            if (attacker is Player) {
                attacker.backupStatus()
                val decs = attacker.equipmentsArray
                        .sliceArray(0..1)
                        .filterNotNull()
                if (!decs.isEmpty()) {
                    decs.forEach {
                        it.affect(attacker)
                    }
                    animations.add(attacker.diffToAnimation())
                }
            }

            if (attacker.isPoisoned) {
                attacker.backupStatus()
                if (attacker.hp == 1) {
                    attacker.hp = 0
                } else {
                    val hp = attacker.hp.toDouble() * 0.75
                    attacker.hp = hp.toInt()
                }
                animations.add(attacker.diffToAnimation())
            }
        }
    }

    override fun update(delta: Long): Boolean {
        if (animations.isEmpty())
            return false
        if (!animations.first().update(delta))
            animations.removeAt(0)
        return !animations.isEmpty()
    }

    override fun draw(canvas: Canvas) {
        if (animations.isEmpty())
            return
        animations.first().draw(canvas)
    }

}