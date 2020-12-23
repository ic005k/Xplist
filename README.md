# PlistEDPlus--开源跨平台的轻量级Plist文件编辑器

写在前面：该软件的原型来自：https://github.com/alpex92/PlistED  ，由于这个原型的存在，不至于使我从头开始码砖，感谢@alpex92！

写这个软件的时候，其实有些犹豫，类似的软件貌似也不少，再加上时间的关系，一直没有付出实质的行动。但写一款Plist编辑器的想法，一直挥之不去，所以最终促成了PlistEDPlus...

## 特征包括但不限于：

* 简洁明快易于使用的人机交互，力保功能精炼，不臃肿、不多余

* 开源、跨平台（目前支持win、mac、linux）

* 人性化的搜索功能（a.在展开的节点范围内，键、数据类型、值各列的键盘首字符搜索; b.搜索框全局搜索，一次性展示搜索结果）

* 完全遵循并符合plist文件技术规范，数据类型显示与plist文本完全一致，不做任何转义

* 多标签浏览文件

* 多国语言支持（目前支持中文、俄文、英文）

* 拖拽文件到软件界面打开文件、双击打开文件、文件打开的历史记录显示、自动加载上次打开的文件

* 增加、移除、粘贴、剪切、编辑条目等，全部纳入Undo、Redo策略组，支持撤销和恢复

* 展开和折叠条目的右键菜单，在不同的列，有不同的效果

* 对于data类型的值，悬浮窗自动显示对应的ASCII和Base64的值

* 更改数据类型时，其值会自动随之转换

* 可自由选择某个节点下的子项进行A->Z排序（目前不对数组进行排序）

* 内置更新检查

* 如果当前打开的文件被其他程序修改，则自动提示是否重新装入

* 软件UI自适应各种分辨率，支持winpe、linux livecd等

...

![截图](https://github.com/ic005k/PlistEDPlus/blob/main/plist.png)

### [国内建议使用github加速器下载](https://toolwa.com/github/)

## 讨论区：

[远景pcbeta](http://bbs.pcbeta.com/viewthread-1873474-1-1.html)&nbsp;&nbsp;&nbsp;&nbsp;[insanelymac](https://www.insanelymac.com/forum/topic/345512-open-source-cross-platform-plist-file-editor-plistedplus/)



### 备注：此项目开始于2020年10月
