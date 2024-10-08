# Open_Ra2ob_qt6

## 简介

本项目使用 [Ra2ob](https://github.com/wudi-7mi/ra2ob) 项目提供的数据获取接口，欲打造开源的图形化界面的红警2观战工具，目的是能够实时显示各个玩家的基本信息以及单位数量，同时对进阶的观战功能也有一定的构想。

本人仅仅为开源爱好者，qt、cpp等技术不精，仅在课余时间缓慢地学习与开发。如果有兴趣加入，欢迎提 issue 或者 pull request。

本项目最初由 [ra2viewer](https://github.com/chenguokai/ra2viewer) 项目启发而成立，再次感谢该项目的作者提供的思路。

### 基础功能

- 显示对战玩家基本信息（金钱、电量、国家、颜色、单位数量、建造队列）
- 显示玩家经济情况，下方显示总消费数量和卡钱指示条
- 支持原版和尤复双版本，双版本共有的单位图标可以区别显示
- 支持战网/CNC/兰博三个平台（虽然兰博有自己的观战插件）兰博的回放也支持
- 暂时隐藏观战插件、设置透明比分栏
- 适配各种分辨率的屏幕，可以切换需要显示的显示器

Todo:

- [ ] 增加 2v2, 3v3 的观战

### 使用指南

下载 Release 中的压缩包并解压，打开 open_ra2ob_qt6.exe

也可以百度云下载：[链接](https://pan.baidu.com/s/1lsvh4Wa37kpqbljs12ZgVQ?pwd=r70r) 提取码：r70r 

战网玩家请在设置中将显示补丁设为 8: GDI，选择你显示器的分辨率，并且勾选无边框窗口模式

CNC玩家选择你显示器的分辨率，并且勾选无边框窗口模式

兰博玩家选择你显示器的分辨率，并且选择无边框窗口或者拉伸全屏即可

如果上面的设置没有完成将无法正常使用本工具

设置完成后以观战身份进入游戏，观战界面将会自动显示

## 注意事项

- 如果你在使用中出现了各种各样的问题或者有什么意见和建议，请在软件的“关于”页面点击意见反馈。
- 本项目为开源项目，遵循 AGPL-3.0 开源协议
- 如果你想参与开发或测试，可以加入 qq 群 945693007

## 项目进度

自 20230916 开始记录：

- 20230916：此前的主菜单、ob 覆盖界面背景已经创建完毕；Ra2ob 能正常显示玩家的信息以及单位数量
- 20231122：Open_Ra2ob_qt6 可以正常显示玩家信息和单位数量
- 20240113：实现建造队列的显示；支持尤复版本；增加底部经济情况分析；完善菜单和持久化设置存储
- 20240824: 增强了比分板；游戏暂停界面现在会自动隐藏观战界面

## 项目相关资料和安排

[我的 Notion](https://glib-mahogany-8be.notion.site/Open_Ra2_qt6-7c0b95c9b6e54cc1bbd68dda9e7a2193?pvs=74)

## 进一步的开发指南

待更新......