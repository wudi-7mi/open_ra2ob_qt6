# 编写 Open_ra2ob_qt6 的一些笔记：

## 偏移量

```
基础偏移：
	Fixed:						0xa8b230
	ClassBaseArray:		0xa8022c
	PlayerBaseArray:	0x1180

五项偏移：
	Building:		0x5554
	Unit(Tank):	0x5568
	Infantry:		0x557c
	Aircraft:		0x5590
	HouseType:  0x34


苏军：
Building:
苏军重工 Soviet War Factory 0x38

Tank:
武装采矿车 War Miner 0x4
犀牛坦克 Rhino Tank 0xc
防空车 Flak Track 0x44
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


## 疑问

- 盟军和苏军的工程师是一样的单位吗 (不是 )

## 想象到的功能

- 不同分辨率下的适配
- 快捷键切换需要看到的单位数量
- 自定义计分板
- 显示所有的单位 包括对面阵营的单位
- 伞兵时间的显示
- 科技建筑数量显示
- 牛车挂机显示
- 标注升星（渗透）
- 超武冷却时间
- 按照血量、等级显示单位
- 尤里控制的单位计数？
