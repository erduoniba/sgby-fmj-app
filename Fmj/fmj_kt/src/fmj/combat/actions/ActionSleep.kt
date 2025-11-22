package fmj.combat.actions

import fmj.characters.FightingCharacter

open class ActionNop(target: FightingCharacter): ActionSingleTarget(target, target) {
    override fun update(delta: Long): Boolean {
        return false
    }
}