# 崩铁战斗模拟

## 当前状态
⚠️ **注意**：此项目尚未完成
（可能永远都不会完成awa）

---

## 战斗配置指南
1. 配置文件为 `battle.txt`。
2. **角色站位规则**：
   - 前4个位置为自机角色（己方）。
   - 后5个位置为敌方角色。
   - 若有空位，需填写 `0` 占位。

---

## 角色配置说明
1. **角色ID规则**：
   - 必须与 `Data` 目录下的文件名**完全一致**（包括大小写）。
   - 仅允许使用**英文字母或数字**作为ID。
   
2. **属性文件格式**：
   - 必须严格按照顺序和指定格式编写角色属性。
   - 技能部分以 `end` 结尾，若为最后一个技能则标记为 `EOF`。

3. **技能指令参考**：
   - 详细指令请查看 `commands.txt` 文件。

---

## 注意事项
- 自定义角色时，请确保遵守上述规则，否则可能导致程序无法正确读取配置。
