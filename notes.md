# 编写 Open_ra2ob_qt6 的一些笔记：

```
单位显示的顺序

苏军：
武装采矿车 War miner
犀牛坦克 Rhino tank
防空车 Flak track
苏军警犬 Soviet Attack dog
动员兵 Conscript
工程师 Engineer
苏军重工 Soviet war factory
蜘蛛 Terror Drone
章鱼 Giant squid
基洛夫 Kirov airship
苏军基地 Soviet MCV
苏军运输船 Amphibious transport
天启坦克 Apocalypse tank
无畏级战舰 Dreadnought
潜艇 Typhoon attack submarine
尤里 Yuri
海螺 Sea Scorpion
磁暴步兵 Tesla trooper
疯狂伊文 Crazy Ivan
v3火箭 V3 rocket launcher

（特殊）
辐射 Desolator
恐怖分子 Terrorist
磁能坦克 Tesla tank


盟军：
超时空矿车 Chrono Miner
灰熊坦克 Grizzly Battle Tank
盟军警犬 Allied Attack dog
美国大兵 GI
盟军工程师 Engineer
盟军重工 Allied war factory
火箭飞行兵 Rocketeer
盟军运输船 Amphibious transport
海豚 Dolphin
驱逐舰 Destoryer
神盾 Aegis cruiser
航母 Aircraft carrier
多功能车 Infantry Fighting Vehicle
工程师 Engineer
超时空兵 Chrono legionnaire
入侵者 Harrier
幻影坦克 Mirage tank
光棱坦克 Prism tank
盟军基地 Allied MCV
海豹 Navy SEAL
夜莺直升机 NightHawk transport
间谍 Spy
谭雅 Tanya

（特殊）
黑鹰战机 
巨炮 Grand cannon
坦克杀手 Tank destoyer
狙击手 Sniper

科技：
尤里改 Yuri prime
超时空伊文 Chrono Ivan
心灵突击队 Psi commando
超时空突击队 Chrono commando
```

## 颜色

```
黄		#ffff00
红		#ff0000
蓝		#0000ff
绿		#00ff00
橙		#e7b325
天		#0088ff
紫		#9000ff
粉		#f19ec2
```


## 偏移量

```
四项偏移：
	Building:		0x5554
	Unit(Tank):	0x5568
	Infantry:		0x557c
	Aircraft:		0x5590


苏军：
Building:
苏军重工 Soviet war factory 0x38

Tank:
武装采矿车 War miner 0x4
犀牛坦克 Rhino tank 0xc
防空车 Flak track 0x44
磁能坦克 Tesla tank 0x74
天启坦克 Apocalypse tank 0x20
v3火箭 V3 rocket launcher 0x38
蜘蛛 Terror Drone 0x40
章鱼 Giant squid 0x60
基洛夫 Kirov airship 0x3c
苏军基地 Soviet MCV 0x68
无畏级战舰 Dreadnought 0x58
潜艇 Typhoon attack submarine 0x4c
海螺 Sea Scorpion 0x90
苏军运输船 Amphibious transport 0x10
自爆卡车 Demolition truck 0xa4

Infantry:
苏军警犬 Soviet Attack dog 0x24
动员兵 Conscript 0x4
工程师 Soviet Engineer 0x6c
磁暴步兵 Tesla trooper 0x8
疯狂伊文 Crazy Ivan 0x1c
尤里 Yuri 0x18
辐射 Desolator 0x20
恐怖分子 Terrorist 0x68

Aircraft:


盟军：
Building:
盟军重工 Allied war factory 0x1c
巨炮 Grand cannon 0x100

Tank:
超时空矿车 Chrono Miner 0x84
灰熊坦克 Grizzly Battle Tank 0x24
幻影坦克 Mirage tank 0x94
光棱坦克 Prism tank 0x88
盟军基地 Allied MCV 0x0
盟军运输船 Amphibious transport 0x54
驱逐舰 Destoryer 0x48
海豚 Dolphin 0x64
神盾 Aegis cruiser 0x50
航母 Aircraft carrier 0x34
夜莺直升机 NightHawk transport 0x5c
坦克杀手 Tank destoyer 0x6c
多功能车 Infantry Fighting Vehicle 0x98

Infantry:
盟军警犬 Allied Attack dog 0x70
美国大兵 GI 0x0
盟军工程师 Allied Engineer 0xc
超时空兵 Chrono legionnaire 0x3c
间谍 Spy 0x40
谭雅 Tanya 0x60
狙击手 Sniper 0x54
火箭飞行兵 Rocketeer 0x10

Aircraft:
入侵者 Harrier 0x4
黑鹰战机 Black Eagle 0x1c


科技：
尤里改 Yuri prime 0x50
超时空伊文 Chrono Ivan 0x4c
心灵突击队 Psi commando 0x48
超时空突击队 Chrono commando 0x44
```


## 疑问

- 盟军和苏军的工程师是一样的单位吗 (不是 )

## 想象到的功能

- 快捷键切换需要看到的单位数量
- 自定义计分板
- 显示所有的单位 包括对面阵营的单位
