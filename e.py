from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.action_chains import ActionChains

class OzonScraper:
    def __init__(self, url):
        options = Options()
        options.add_argument("--disable-blink-features=AutomationControlled")
        options.add_argument("--user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36")
        self.driver = webdriver.Chrome(options=options)
        self.driver.execute_cdp_cmd("Page.addScriptToEvaluateOnNewDocument", {
            "source": """
                Object.defineProperty(navigator, 'webdriver', {
                    get: () => undefined
                })
            """
        })
        self.driver.get(url)
        self.products = []
        WebDriverWait(self.driver, 10).until(EC.presence_of_element_located((By.TAG_NAME, 'body')))

    def scroll_page_and_wait(self):
        self.driver.find_element(By.TAG_NAME, 'body').send_keys(Keys.END)

    def extract_product_details(self, product_element, item_number):
        try:
            ActionChains(self.driver).key_down(Keys.SHIFT).click(product_element).key_up(Keys.SHIFT).perform()
            WebDriverWait(self.driver, 10).until(EC.number_of_windows_to_be(2))
            original_window = self.driver.current_window_handle
            new_window = [window for window in self.driver.window_handles if window != original_window][0]
            self.driver.switch_to.window(new_window)
            WebDriverWait(self.driver, 10).until(EC.presence_of_element_located((By.TAG_NAME, 'body')))

            price = self.driver.find_element(By.XPATH, "//*[contains(text(), '₽')]").text.strip()
            sku = self.driver.find_element(By.XPATH, "//*[contains(text(), 'Артикул')]").text.split("Артикул:")[-1].strip()
            print(f"Товар #{item_number}: Артикул: {sku}, Цена: {price}")
            self.products.append({"SKU": sku, "Price": price})

            self.driver.close()
            self.driver.switch_to.window(original_window)
        except Exception as e:
            print(f"Ошибка при извлечении данных товара: {e}")

    def scrape_products(self, scroll_count):
        item_number = 1
        try:
            paginator = WebDriverWait(self.driver, 10).until(
                EC.presence_of_element_located((By.ID, 'paginator'))
            )
            print("Теги и текст внутри paginator:")
            for child in paginator.find_elements(By.XPATH, "./*"):
                tag_name = child.tag_name
                text_content = child.text.strip()
                print(f"<{tag_name}>: {text_content}")

        except Exception:
            print("Элемент с id='paginator' не найден.")
            return

        for _ in range(scroll_count):
            self.scroll_page_and_wait()
            product_elements = paginator.find_elements(By.TAG_NAME, 'div')
            for product_element in product_elements:
                self.extract_product_details(product_element, item_number)
                item_number += 1


    def quit(self):
        self.driver.quit()

if __name__ == "__main__":
    url = "https://www.ozon.ru/seller/vink-1340217/products/?miniapp=seller_1340217"
    scraper = OzonScraper(url)
    scraper.scrape_products(scroll_count=10)
    print(*scraper.products, sep="\n")
    scraper.quit()
