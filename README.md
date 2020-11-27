# C/C++ Test

在軟體工程中，應該在開發時，就能確保品質，也就是在源頭保障品質，其中測試就是一個很好的方法。測試又分為：單元測試、整合測試...等，本篇將講解在 C/C++ 語言中一些實用的測試技巧，提升軟體品質。 想了解更多關於測試的說明，可參考文章最下方的[參考文獻](#Reference)。

在 C/C++ 中，我們可以藉由使用 google 提供的 gtest 與 gmock， 撰寫出簡潔的測試程式，提升測試程式的可讀性，並與外部元件(database, server)隔離，專注於當下撰寫的程式邏輯上。

以下例子說明如何將 gtest 與 gmock 使用於專案中. 我們會使用 cmake 作為編譯工具。cmake 用來幫助產生 makefile，讓 developer 使用更直覺的語法就能產生複雜的 makefile，而不用花大把的時間研究如何撰寫 makefile。

- gtest - 提供方便使用的 macro，可快速撰寫「比較」的程式碼
- gmock - 提供 mock 功能，可模擬外部元件。

完整範例可參考 [src](./src)，以下節錄重點片段

## Demo

### Install googletest

首先我們先將 googletest clone 於專案目錄下，

```sh
git clone https://github.com/google/googletest
```

> Tips:
> 一般在專案中應採用 git module 的方式，將第三方函式庫連結到專案中，而非整個 source code 都加入，可以帶來許多方便性，如：減少專案的體積、隨時更新第三方函式庫版本、追蹤第三方函式庫發展。這裡我們為了方便說明，只簡單使用 git clone。

### Build

```sh
mkdir build
cd build
cmake .. && make
```

### Run

```sh
./runTests
```

執行後，即可在 Terminal 看到那富有色彩的測試結果。

## 說明

使用 google test 的專案需在 CMakeLists.txt 中加入 googletest 路徑

```cmake
project(TEST)

# ...
# add googletest path to project
add_subdirectory (${PROJECT_SOURCE_DIR}/googletest)
# Add your sources
# ...

target_link_libraries(runTests gmock)
```

其中最後一行

```cmake
target_link_libraries(runTests gmock)
```

將 gmock 連結到專案中，如果只打算使用 gtest，則可以改用 gtest。
如果不需要自訂 main 函式，可以連結 `gtest_main` 或 `gmock_main`，就不需要撰寫 main 函式。

例如只打算使用 gtest 且不需要自訂 main，則可以

```cmake
target_link_libraries(runTests gtest gtest_main)
```

將會自動產生 main 函式，因此測試檔只需撰寫測試程式碼。
> Tips:
> 以本專案為例的話，就是若連結 gtest_main，則可以省略 test/gmock_example_c_version.cc 中的 30~34 行。


### gtest 相關語法

以下是 gtest_example.cc 中的範例

```cc
#include <gtest/gtest.h>

#include "mail.h"

TEST(Create, create_a_notification) {
  Notification notification = createNotification("Hello", "google test");
  EXPECT_STREQ(notification.title, "Hello");
  EXPECT_STREQ(notification.message, "google test");
  EXPECT_GT(notification.timestamp, 0);
}
```

第 1 行引入 gtest 標頭檔，接著引入我們想測試的函式標頭檔。

一個測試由 `TEST` 所包圍，第一個參數為 Case name，第二個參數為 name. 我們可以在同一個檔案中，撰寫多條 case，但 case name 與 name 的組合必須是唯一。值的比較使用

- `EXPECT_*` 為非 Fatal 的判斷，指即便判斷失敗，也會繼續往下執行。
- `ASSERT_*` 為 Fatal 的判斷，指失敗了，程式將會終止，不在繼續往下執行。

> Tips:
> ASSERT 的使用上有一點要注意，若將 ASSERT 放於函式之中，則該函式回傳值必須是 void，我們可以藉由將 ASSERT 單獨使用回傳值是 void 的函式包起來，如
>
> ```cc
> int verifyAndIncrease(int x);
> void paramISNotZero(int x) {
>   ASSERT_NE(x, 0);
> }
>
> TEST(Example, Assert) {
>   int x = 1;
>   verifyAndIncrease(x);
> }
>
> int verifyAndIncrease(int x) {
>   paramISNotZero(x);
>   return x + 1;
> }
> ```

主要可以進行的判斷有

| Fatal assertion          | Nonfatal assertion       | Verifies       |
| ------------------------ | ------------------------ | -------------- |
| `ASSERT_EQ(val1, val2);` | `EXPECT_EQ(val1, val2);` | `val1 == val2` |
| `ASSERT_NE(val1, val2);` | `EXPECT_NE(val1, val2);` | `val1 != val2` |
| `ASSERT_LT(val1, val2);` | `EXPECT_LT(val1, val2);` | `val1 < val2`  |
| `ASSERT_LE(val1, val2);` | `EXPECT_LE(val1, val2);` | `val1 <= val2` |
| `ASSERT_GT(val1, val2);` | `EXPECT_GT(val1, val2);` | `val1 > val2`  |
| `ASSERT_GE(val1, val2);` | `EXPECT_GE(val1, val2);` | `val1 >= val2` |

關於字串的判斷有

| Fatal assertion                 | Nonfatal assertion              | Verifies                                                 |
| ------------------------------- | ------------------------------- | -------------------------------------------------------- |
| `ASSERT_STREQ(str1, str2);`     | `EXPECT_STREQ(str1, str2);`     | the two C strings have the same content                  |
| `ASSERT_STRNE(str1, str2);`     | `EXPECT_STRNE(str1, str2);`     | the two C strings have different contents                |
| `ASSERT_STRCASEEQ(str1, str2);` | `EXPECT_STRCASEEQ(str1, str2);` | the two C strings have the same content, ignoring case   |
| `ASSERT_STRCASENE(str1, str2);` | `EXPECT_STRCASENE(str1, str2);` | the two C strings have different contents, ignoring case |

測試案例儘量彼此之間互不干擾。

有的時候我們希望一些測試使用相同的情境(測試數據)，則可以使用 `TEST_F` 這個 macro，其稱為 Test Fixture，如下範例

```cc
class NotificationTest : public testing::Test {
 protected:
  const char *title = "Hi";
  void SetUp() override {
    printf("SetUp runs before each case.\n");
  }

  void TearDown() override {
    printf("TearDown runs after each case.\n");
  }
};

TEST_F(NotificationTest, create_a_notification2) {
  Notification notification = createNotification(title, "google test2");
  EXPECT_STREQ(notification.title, title);
  EXPECT_STREQ(notification.message, "google test2");
}

TEST_F(NotificationTest, create_a_notification3) {
  Notification notification = createNotification(title, "google test3");
  EXPECT_STREQ(notification.title, title);
  EXPECT_STREQ(notification.message, "google test3");
}
```

我們可以看到 `NotificationTest` 繼承了 `testing::Test`，因此我們可以對一群需要進行相同前置動作、資料的測試使用 NotificationTest，如上述的兩個 `TEST_F`。其中，`SetUp` 與 `TearDown` 方法，可以讓我們設定每一個測試執行前與執行後的動作。

更多資訊可參考此[文件](https://github.com/google/googletest/blob/master/googletest/docs/primer.md#binary-comparison)

## gmock 相關說明

有的時候專案會跟外部元件互動，例如與 Server 溝通、存取資料庫，但總不可能再寫一個假的 Server 來進行測試，而如果直接與外部元件溝通，又喪失了單元測試的意義。因此我們可以利用 gmock 幫我們模擬實際部份函式，讓程式與外部元件解偶。

> Tips:
> 為了充份運用 gmock，一般我們會將需要與外部互動的部份抽出成模組，如此一來只要讓 gmock 去扮演這個模組，就能完全與外部元件隔離。

## gmock for C

gmock 本身是針對 C++ 的 mock 框架，但我們可以透過在外面包裹一層 C，實現 mock C 的功能。

例如，我們想要測試下面的 `sendNotification` 函式

```cc
#ifndef MAIL_H_
#define MAIL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char title[16];
  char message[256];
  long timestamp;
} Notification;

Notification createNotification(const char *title, const char *message);

int sendNotification(Notification *notification);

#ifdef __cplusplus
}
#endif

#endif
```

但我們檢查其源碼，會發現他呼叫了 `network.h` 中的 `sendToServer` 函式，該函式實現了實際送出的邏輯。

接著就是 gmock 出場的時候了，首先建立 mockNetworkInterface.h

```cc
#include <gmock/gmock.h>

#include "network.h"

class MockNetwork {
 public:
  MOCK_METHOD2(sendToServer, int(char *, int));
};

using std::unique_ptr;

extern unique_ptr<MockNetwork> mockNetwork;
```

其中，unique_ptr 為 Smart pointer 的一種，其可以讓該 pointer 指向的對象，不再被參考時，會自動由系統回收，不需要自行回收，更進一步的說明，可自行上網查詢。

以上我們利用 extern 的方式宣告有此 pointer 存在，實際宣告位置位於 mockNetwork.cc

```cc
#include "mockNetworkInterface.h"

#include <stdio.h>

#include "network.h"

unique_ptr<MockNetwork> mockNetwork;

int sendToServer(char *buf, int size) {
  return mockNetwork->sendToServer(buf, size);
}
```

我們藉由一層 C 的函式(與 network.h 宣告的函式相同)

```cc
int sendToServer(char *buf, int size) {
  // call c++ method
}
```

將 gmock 模擬的 C++ 函式包裹，從而實現 mock C 的功能，至此還差最後一步，就是實體化 mockNetwork 物件。撰寫於 gmock_example_c_version.cc

```cc
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mail.h"
#include "mockNetworkInterface.h"

using std::unique_ptr;
using ::testing::_;
using ::testing::Environment;
using ::testing::Return;

TEST(Send, send_a_notification_expected_success) {
  Notification notification = createNotification("Hello", "google test");
  EXPECT_CALL(*mockNetwork, sendToServer(_, _)).WillOnce(Return(0));

  int result = sendNotification(&notification);
  EXPECT_EQ(result, 0);
}

class TestEnvironment : public Environment {
 public:
  void SetUp() {
    mockNetwork = unique_ptr<MockNetwork>(new MockNetwork);
  }
  void TearDown() {
    mockNetwork = nullptr;
  }
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);
  return RUN_ALL_TESTS();
}
```

我們透過繼承 Environment，將 mockNetwork 物件的創建與刪除，加入其中，再透過 `::testing::AddGlobalTestEnvironment` 將初始環境的建置與銷毀加入其中，確保可以在進行任何測試之前，先將 mockNetwork 物件建立。

最後我們就可以透過 EXPECT_CALL 達成與 gmock 模擬函式的功能。

## Reference

1. [googletest](https://github.com/google/googletest)
