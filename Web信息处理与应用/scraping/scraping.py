# coding=<utf-8>
import os
from urllib.request import urlopen
from urllib import error
from bs4 import BeautifulSoup
import requests
from urllib.parse import urljoin

# save location
os.makedirs("./html", exist_ok=True)
DIRECTION = "./html/"

# number of scraping page
GLOBAL_STEP = 1000
# scraping seed
seed_url = "https://www.ustc.edu.cn/"
now_url = seed_url
# prepare list
history = []
target = []

for i in range(GLOBAL_STEP):
    # open page
    try:
        page = urlopen(now_url).read().decode('utf-8')
        history.append(now_url)
    
        # download page
        ret = requests.get(now_url, stream = True)
        with open(DIRECTION + str(i) + '.html', 'wb') as f:
            for chunk in ret.iter_content(chunk_size=1024):
                f.write(chunk)

        # find all links
        soup = BeautifulSoup(page, features='lxml')
        print(i, soup.find("title"), "url:", now_url)
        all_href = soup.find_all('a')
        if len(all_href) != 0 :
            try :
                all_href = [k['href'] for k in all_href]
                # processing relative routine url
                for url in all_href:
                    if url[0:1] == '/':
                        all_href.append(urljoin(now_url, url))
                        all_href.remove(url)
                target.extend(all_href)
            # 网页中包含的格式不一样，all_href中项的字典可能为空，会出现KeyError异常，暂不处理
            except KeyError :
                pass
    
    # 部分网页不能被utf-8解码，会出现UnicodeDecodeError异常，暂不处理
    except UnicodeDecodeError:
        pass
    # 部分网页无法打开，urllib.error.URLError
    except error.URLError:
        pass
    # 主机中的软件可能中止了一个已建立的连接
    except ConnectionAbortedError:
        pass

    # prepare next url
    if len(target) != 0:
        next_url = target.pop(0)
    while True:
        if next_url in history or next_url[0:4] != "http":
            if len(target) != 0:
                next_url = target.pop(0)
            else:
                print("target list is empty!")
        else:
            break
    now_url = next_url

    if len(target) == 0:
        print("target list is empty!")
        break
