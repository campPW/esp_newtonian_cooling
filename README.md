# esp_newtonian_cooling
Esp32 fan system meant for use with Newtonian reflectors.

**Why is cooling necessary?**

Small temperature differences between the telescope mirror and the ambient air result in image degradatation. This loss of resolution becomes more apparent when observing at higher magnifications. Given most mirrors will not radiate their heat away quickly enough to  match the decreasing night time temperature, a cooling solution is required. A great article on the necessity of cooling for newtonian reflectors can be found [here!](https://www.fpi-protostar.com/bgreer/fanselect.htm)

**Dependencies**

ESP-IDF stable version (4.0). [ESP-IDF Get Started](https://docs.espressif.com/projects/esp-idf/en/stable/api-guides/build-system.html#idf-py)

**Hardware**

2x DHT22 temperature and humidity sensors: [DHT22](https://www.aliexpress.com/item/32901733917.html?spm=a2g0o.productlist.0.0.2852bd0cHSo2Kb&algo_pvid=980de48b-a609-4085-9dd3-a356819071b6&algo_expid=980de48b-a609-4085-9dd3-a356819071b6-0&btsid=0ab6f82215819887757151207e16a8&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_tm)

1x Esp32 DOIT Devkit v1: [Esp32 DOIT Devkit v1 board](https://www.aliexpress.com/item/32799253567.html?spm=a2g0o.productlist.0.0.7a9d7508kSwOZm&algo_pvid=2f43cc4a-fbef-46d8-b716-5135b8f98bb4&algo_expid=2f43cc4a-fbef-46d8-b716-5135b8f98bb4-0&btsid=0ab50f4415819889697527557e9416&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)

1x 5v 10 cfm fan: [5v fan](https://www.mouser.com/Thermal-Management/Fans-Blowers/_/N-axg88) (select a fan with appropriate airflow rating for your mirror thickness. See above article on telescope cooling.)

1x 3.3v relay module [3.3v relay module](https://www.amazon.com/gp/product/B07P73PHQY/ref=ppx_yo_dt_b_asin_image_o00_s00?ie=UTF8&psc=1)

**AWB OneSky Specific Setup**

The telescope I'm using is the popular and highly portable AWB OneSky (https://shop.astronomerswithoutborders.org/products/awb-onesky-reflector-telescope)


