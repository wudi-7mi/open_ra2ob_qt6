# Open_Ra2ob_qt6

## 简介

本项目使用 [Ra2ob](https://github.com/wudi-7mi/ra2ob) 项目提供的数据获取接口，欲打造开源的图形化界面的红警2观战工具，目的是能够实时显示各个玩家的基本信息以及单位数量，同时对进阶的观战功能也有一定的构想。

本人仅仅为开源爱好者，qt、cpp等技术不精，仅在课余时间缓慢地学习与开发。如果有兴趣加入，欢迎提 issue 或者 pull request。

本项目最初由 [ra2viewer](https://github.com/chenguokai/ra2viewer) 项目启发而成立，再次感谢该项目的作者提供的思路。

## 偏移量

```
基础偏移：
	Fixed:            0xa8b230
	ClassBaseArray:   0xa8022c
	PlayerBaseArray:  0x1180

五项偏移：
	Building:    0x5554
	Unit(Tank):  0x5568
	Infantry:    0x557c
	Aircraft:    0x5590
	HouseType:   0x34


苏军：
Building:
苏军重工 Soviet War Factory 0x38

Tank:
武装采矿车 War Miner 0x4
犀牛坦克 Rhino Tank 0xc
防空履带车 Flak Track 0x44
磁能坦克 Tesla Tank 0x74
天启坦克 Apocalypse Tank 0x8
v3火箭 V3 Rocket Launcher 0x38
蜘蛛 Terror Drone 0x40
章鱼 Giant Squid 0x60
基洛夫 Kirov Airship 0x3c
苏军基地 Soviet MCV 0x68
无畏级战舰 Dreadnought 0x58
潜艇 Typhoon Attack Submarine 0x4c
海螺 Sea Scorpion 0x90
苏军运输船 Amphibious Transport 0x10
自爆卡车 Demolition Truck 0xa4

Infantry:
苏军警犬 Soviet Attack Dog 0x24
动员兵 Conscript 0x4
工程师 Soviet Engineer 0x6c
磁暴步兵 Tesla Trooper 0x8
疯狂伊文 Crazy Ivan 0x1c
尤里 Yuri 0x18
辐射 Desolator 0x20
恐怖分子 Terrorist 0x68
防空步兵 Flak Trooper 0x5c

Aircraft:

盟军：
Building:
盟军重工 Allied War Factory 0x1c
巨炮 Grand Cannon 0x100

Tank:
超时空矿车 Chrono Miner 0x84
灰熊坦克 Grizzly Battle Tank 0x24
幻影坦克 Mirage Tank 0x94
光棱坦克 Prism Tank 0x88
盟军基地 Allied MCV 0x0
盟军运输船 Amphibious Transport 0x54
驱逐舰 Destoryer 0x48
海豚 Dolphin 0x64
神盾 Aegis Cruiser 0x50
航母 Aircraft Carrier 0x34
夜莺直升机 NightHawk Transport 0x5c
坦克杀手 Tank Destoyer 0x6c
多功能车 Infantry Fighting Vehicle 0x98

Infantry:
盟军警犬 Allied Attack Dog 0x70
美国大兵 GI 0x0
盟军工程师 Allied Engineer 0xc
超时空兵 Chrono Legionnaire 0x3c
间谍 Spy 0x40
谭雅 Tanya 0x60
狙击手 Sniper 0x54
火箭飞行兵 Rocketeer 0x10

Aircraft:
入侵者 Harrier 0x4
黑鹰战机 Black Eagle 0x1c


科技：
尤里改 Yuri Prime 0x50
超时空伊文 Chrono Ivan 0x4c
心灵突击队 Psi Commando 0x48
超时空突击队 Chrono Commando 0x44
```

## 项目进度

自 20230916 开始记录：

- 20230916：此前的主菜单、ob 覆盖界面背景已经创建完毕；Ra2ob 能正常显示玩家的信息以及单位数量。
- 20231122：Open_Ra2ob_qt6 可以正常显示玩家信息和单位数量

## Todo List

Ra2ob 类相关：

- [ ] 建造队列显示 
- [ ] 超武、伞兵冷却时间显示
- [ ] 科技建筑数量显示
- [ ] 标注升星（渗透）
- [ ] 尤里控制的单位计数

GUI 相关：

- [ ] 不同分辨率下的适配
- [x] 控制显示器的切换
- [ ] 自定义计分板样式
- [ ] 控制观战界面的透明度、显示与否

综合：

- [ ] 快捷键切换需要看到的单位数量
- [ ] 按照血量、等级显示单位
