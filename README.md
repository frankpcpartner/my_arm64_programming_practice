# my_arm64_programming_practice

This repository contains the implementation of exercises from the book ***Arm64体系结构编程与实践***. 
It serves as a practical guide to understanding the ARM architecture and writing assembly programs.

## 📘 Book Overview

***Arm64体系结构编程与实践*** introduces the ARM instruction set, assembly syntax, memory operations, flow control, system calls, and more. 
It is widely used as a resource for learning low-level programming on ARM systems.

👉 [View the book on Tenlong Bookstore](https://www.tenlong.com.tw/products/9787115582102)

## 📂 Project Structure

The repository is organized by chapters and exercise topics:
- Chapter 2:
  * lab01: 輸出 “Welcome BenOS!”

- Chapter 3:
  * lab01: 熟悉 MOV 和 LDR 指令
  * lab02: 前變基與後變基尋址模式 1
  * lab03: 前變基與後變基尋址模式 2
  * lab04: PC 相對地址尋址
  * lab05: 實作 memcpy() 函數
  * lab06: LDP 和 STP 指令的使用

- Chapter 4:
  * lab01: 測試 ADDS 和 CMP 指令的 C 標誌位
  * lab02: 條件標誌位的使用
  * lab03: 測試 ANDS 指令以及 Z 標誌位
  * lab04: 測試位段操作指令
  * lab05: 使用位段指令來讀取寄存器

- Chapter 5:
  * lab01: CMP 和 CMN 指令
  * lab02: 條件選擇指令
  * lab03: 子函數跳轉

- Chapter 6:
  * lab01: 測試 ADRP 和 LDR 偽指令
  * lab02: ADRP 和 LDR 偽指令的陷阱
  * lab03: LDXR 和 STXR 指令的使用 1
  * lab04: LDXR 和 STXR 指令的使用 2

- Chapter 7:
  * lab01: 在匯編中實現串口輸出功能

- Chapter 8:
  * lab01: 匯編語言練習——求最大數
  * lab02: 透過 C 調用匯編函數
  * lab03: 透過匯編語言調用 C 函數
  * lab04: 使用匯編偽操作來實現一張表
  * lab05: 匯編宏的使用

- Chapter 9:
  * lab02: 輸出每個段的內存佈局
  * lab03: 加載地址不等於運行地址
  

- Chapter 10:
  * lab01: 實作簡單的 memcpy 函數
  * lab02: 使用匯編符號名撰寫內嵌匯編代碼
  * lab03: 使用內嵌匯編代碼完善 __memset_16bytes 函數
  * lab04: 使用內嵌匯編代碼與宏
  * lab05: 實作讀寫系統寄存器的宏
  * lab06: 使用 goto 模板的內嵌匯編函數

- Chapter 11:
  * lab01: 切換至 EL1
  * lab02: 建立異常向量表
  * lab03: 尋找 Raspberry Pi 4B 上觸發異常的指令
  * lab04: 解析數據異常資訊

- Chapter 12:
  * lab01: 在 Raspberry Pi 4B 上實作通用定時器中斷
  * lab02: 使用匯編函數保存與恢復中斷現場


- Chapter 14:
  * lab01: 建立恆等映射
  * lab02: 為什麼 MMU 無法運行
  * lab03: 實作一個 MMU 頁表的轉儲功能
  * lab04: 修改頁面屬性導致系統死機
  * lab05: 使用匯編建立恆等映射並打開 MMU
  * lab06: 驗證 LDXR 和 STXR 指令
  * lab07: 使用 AT 指令

- Chapter 15:
  * lab01: 枚舉高速緩存
  * lab02: 清理高速緩存

- Chapter 16:
  * lab01: 高速緩存偽共享
  

- Chapter 21:
  * lab01: 觀察棧佈局
  * lab02: 建立進程
  * lab03: 進程調度

Each section contains commented source code for clarity and learning purposes.

## 🛠️ Development Environment

- Hardware Development Platform: Raspberry Pi 3B+
- Software Emulation Platform: QEMU 6.2
- Host Development Machine: Ubuntu 22.04.5 LTS
- MicroSD Card and a Card Reader
- USB-to-Serial Cable

