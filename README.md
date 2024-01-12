# Open_Ra2ob_qt6

## 简介

本项目使用 [Ra2ob](https://github.com/wudi-7mi/ra2ob) 项目提供的数据获取接口，欲打造开源的图形化界面的红警2观战工具，目的是能够实时显示各个玩家的基本信息以及单位数量，同时对进阶的观战功能也有一定的构想。

本人仅仅为开源爱好者，qt、cpp等技术不精，仅在课余时间缓慢地学习与开发。如果有兴趣加入，欢迎提 issue 或者 pull request。

本项目最初由 [ra2viewer](https://github.com/chenguokai/ra2viewer) 项目启发而成立，再次感谢该项目的作者提供的思路。

## 相关设置

### 快捷键

```
全局热键

Ctrl Alt H 隐藏观战界面
Ctrl Alt PageUp 使比分栏更清晰
Ctrl Alt PageDown 使比分栏更透明
Ctrl Alt J 隐藏下方经济情况栏

界面快捷键

Ctrl S 切换观战屏幕
```



### 单位显示顺序

```
Index=1
	Soviet War Factory
	Allied War Factory
	Yuri War Factory
Index=2
	War Miner
	Chrono Miner
	Slave Miner
Index=3
	Rhino Tank
	Grizzly Battle Tank
	Lasher Tank
Index=4
	Soviet Attack Dog
	Allied Attack Dog
	Brute
Index=5
	Conscript
	GI
	Initiate
Index=6
	Soviet Engineer
	Allied Engineer
	Yuri Engineer
Index=7
	Desolator
	Rocketeer
Index=8
	Harrier
	Black Eagle
Index=9
	Typhoon Attack Submarine
	Destroyer
	Yuri Amphibious Transport
Index=10
	Dreadnought
	Aircraft Carrier
Index=11
	Grand Cannon
```



## 项目进度

自 20230916 开始记录：

- 20230916：此前的主菜单、ob 覆盖界面背景已经创建完毕；Ra2ob 能正常显示玩家的信息以及单位数量
- 20231122：Open_Ra2ob_qt6 可以正常显示玩家信息和单位数量
- 2024011x：实现建造队列的显示；支持尤复版本；增加底部经济情况分析

## Todo List

Ra2ob 相关：

- [x] 建造队列显示 
- [x] 检测是否为玩家(安全性设置)
- [x] 科技建筑数量显示(单位加血显示)
- [ ] 游戏版本检测
- [ ] 整局游戏的检测(如果可以的话加入赛后总结)

GUI 相关：

- [ ] 不同分辨率下的适配(目前实现了1k和2k)
- [x] 控制显示器的切换
- [ ] 计分板
- [x] 控制观战界面的透明度、显示与否
- [ ] 多语言适配

综合：

- [x] 全局快捷键
- [x] 卡钱条显示
- [x] 安全性设置
- [ ] 文档和主页建立
- [ ] 布局参数说明图
- [ ] 菜单栏的编写
- [ ] 项目打包设置(管理员权限)