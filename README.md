# TBoy
![alt TBoy](https://github.com/channel2007/TBoy_ZooCube/blob/master/art/TBoy.jpeg "TBoy")

# 研發日誌
* 2023/05/21 新增Pong連線遊戲
* 2023/05/21 調整目錄路徑
* 2021/08/23 新增動物方塊韌體檔案(firmware目錄)
* 2021/07/23 修正遊戲圖形顏色不正確問題
* 2021/06/25 開放EasyEDA顯上線路圖檔案
* 2021/06/24 新增PCB板

# 前言
DIY了幾台網路大神們開源的掌機後，想想也該是時候來弄台原創掌機回饋一下社群了，也就是說新作的這台掌機會開源，希望對有興趣製作掌機的同好有所幫助。

# 準備材料
* TTGO T-Display ESP32 x  1
* 輕觸開關中兩腳 x 6
* 10kΩ電阻 x 6
* 蜂鳴器 x 1
* 麵包板 x 1
* 麵包板線一盒

# 線路圖
![alt TBoy](https://github.com/channel2007/TBoy_ZooCube/blob/master/art/Schematic_TBoy.png "TBoy")

# ZooCube遊戲介紹
* 遊戲玩法
玩家需要在10x20的空間內控制由上往下掉落的的七種方塊，利用旋轉方塊功能使掉落的方塊連成橫線，啟動消除方塊，如果在20列中都無法連成橫線消除方塊，遊戲就告結束。
遊戲中每消除一列會將每列的方塊顏色作累加，一共有七種顏色的方塊，每種顏色收集滿都會發動不同效果的技能。
隨著消除方塊列的數量越多，方塊下降的速度也會隨之變快，背景也會跟著變換圖片。

# 按鍵說明
* 左右按鈕：移動慢慢掉落中的方塊
* 下按鈕：讓方塊快速掉落
* A按鈕：順時鐘旋轉方塊
* B按鈕：逆時鐘旋轉方塊
* 開始按鈕：暫停遊戲
* 選擇按鈕：選項選單

# 使用網路資源
* [THE SPRITERS RESOURCE](https://www.spriters-resource.com/)
* [robsoncouto/arduino-songs](https://github.com/robsoncouto/arduino-songs)

# Pong連線遊戲介紹
* 遊戲玩法
1. 遊戲場地 Pong遊戲的場地是一個矩形，有兩個玩家，一個在上方，一個在下方。每個玩家都有一個橫條（也稱為"板子"或"球拍"），可在水平方向移動。
2. 遊戲物件：遊戲中的主要物件是一個小球，它會在遊戲場地內以一定的速度移動，並且在撞到邊界或玩家的橫條時反彈。
3. 玩家控制：玩家可以左右移動他們的橫條，目的是讓小球反彈回對方的半場。玩家不能超過遊戲場地的邊界。
4. 得分規則：當一方玩家未能成功用橫條接住小球，使其反彈回對方半場時，對方就會得分。也就是說，如果小球碰到你的場地邊界，對方就獲得一分。獲得一定分數的玩家（例如10分）將獲得勝利。
5. 遊戲流程 遊戲開始時，小球從場地中央或隨機的一個點發出，向任一方向移動。遊戲繼續進行，直到一名玩家贏得遊戲，然後開始新的一局

# 按鍵說明
* 上下按鈕：選1P或2P
* 左右按鈕：左右移動板子
* A按鈕：確定、開始遊戲、遊戲中按住加速板子左右移動
* Start按鈕：無敵(球掉落底部自動反彈)
* Select按鈕：自動玩(板子自動追著球的X座標移動)

# PCB板準備材料
* TTGO T-Display ESP32 x 1
* 輕觸開關中兩腳 x 6
* 10kΩ電阻 x 6
* 無源蜂鳴器9*5.5MM  x 1
* 鋰電池充電板模組 TYPE-C USB介面充電保護二合一 x 1
* XH2.54-2P/針座/彎針/白色插件 2.54間距 x 1
* XH2.54-6P/針座/彎針/白色插件 2.54間距 x 1
* 200mAH、3.7V 聚合物鋰電池 x 1
* 滑動開關三腳二檔微型開關 x 1

# EasyEDA線路圖連接
[EasyEDA - TBoy](https://oshwlab.com/channel2007/tboy)

# PCB線路圖
![alt TBoy](https://github.com/channel2007/TBoy_ZooCube/blob/master/art/PCB_TBoySheet.jpeg "TBoy")
![alt TBoy](https://github.com/channel2007/TBoy_ZooCube/blob/master/art/PCB_TBoy.jpeg "TBoy")

# 實機照
![alt TBoy](https://github.com/channel2007/TBoy_ZooCube/blob/master/art/TBoy_Positive.jpg "TBoy")
![alt TBoy](https://github.com/channel2007/TBoy_ZooCube/blob/master/art/TBoy_Negative.jpg "TBoy")

# 版權宣告
* 請勿使用在商業用途上 

# 詳細解說可以參考筆者寫的文章
[【掌機】原創掌機TBoy](https://vocus.cc/article/60bd70f7fd8978000182d0db?fbclid=IwAR3eowbBvcs1HFQSfAkmH9ZDA18dOALh4RtLMgBzNeIG1E7RuylL_PDUpho)

# 官方紛絲團 
[無限升級](https://www.facebook.com/unlimited.upgrade/posts/2840132506240869?notif_id=1617421138749926&notif_t=page_post_reaction&ref=notif)

# 官方Youtube
[無限升級](https://www.youtube.com/channel/UC4reRKznNk1CcjZfxKUdMuw)

