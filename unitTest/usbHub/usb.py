import hid
import json
import time


def send_data_to_hid(vendor_id, product_id, data_list):
    try:
        # 打开 HID 设备
        device = hid.device()
        device.open(vendor_id, product_id)

        for data in data_list:
            # 将数据转换为字节
            data_bytes = data.encode('utf-8')

            chunk_size = 64
            # 分段发送数据
            for i in range(0, len(data_bytes), chunk_size):
                chunk = data_bytes[i:i + chunk_size]
                # 若当前块不足 64 字节，用 0 填充
                if len(chunk) < chunk_size:
                    padding = b'\x00' * (chunk_size - len(chunk))
                    chunk = chunk + padding
                device.write(chunk)
                # 每发送一个数据块后，延时 100 毫秒
                time.sleep(0.1)

        # 关闭设备
        device.close()
        print("数据发送成功")
    except Exception as e:
        print(f"发送数据时出现错误: {e}")


if __name__ == "__main__":
    # 替换为你的设备的厂商 ID 和产品 ID
    vendor_id = 0xFF4A
    product_id = 0x0002

    data_red = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":0,"color":"red" }] }}'
    ]
    data_blue = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":0,"color":"blue" }] }}'
    ]
    data_green = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":0,"color":"green" }] }}'
    ]
    data_yellow = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":0,"color":"yellow" }] }}'
    ]
    data_purple = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":0,"color":"purple" }] }}'
    ]
    data_none = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":0,"color":"none" }] }}'
    ]
    print("red on")
    send_data_to_hid(vendor_id, product_id, data_red)
    time.sleep(2)
    print("green on")
    send_data_to_hid(vendor_id, product_id, data_green)
    time.sleep(2)
    print("blue on")
    send_data_to_hid(vendor_id, product_id, data_blue)
    time.sleep(2)
    print("yellow on")
    send_data_to_hid(vendor_id, product_id, data_yellow)
    time.sleep(2)
    print("purple on")
    send_data_to_hid(vendor_id, product_id, data_purple)
    time.sleep(2)
    print("all off")
    send_data_to_hid(vendor_id, product_id, data_none)
    time.sleep(2)


    data_red = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":1,"color":"red" }] }}'
    ]
    data_blue = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":1,"color":"blue" }] }}'
    ]
    data_green = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":1,"color":"green" }] }}'
    ]
    data_yellow = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":1,"color":"yellow" }] }}'
    ]
    data_purple = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":1,"color":"purple" }] }}'
    ]
    data_none = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":1,"color":"none" }] }}'
    ]
    print("led1 red on")
    send_data_to_hid(vendor_id, product_id, data_red)
    time.sleep(2)
    print("led1 green on")
    send_data_to_hid(vendor_id, product_id, data_green)
    time.sleep(2)
    print("led1 blue on")
    send_data_to_hid(vendor_id, product_id, data_blue)
    time.sleep(2)
    print("led1 yellow on")
    send_data_to_hid(vendor_id, product_id, data_yellow)
    time.sleep(2)
    print("led1 purple on")
    send_data_to_hid(vendor_id, product_id, data_purple)
    time.sleep(2)
    print("led1 all off")
    send_data_to_hid(vendor_id, product_id, data_none)
    time.sleep(2)

    data_red = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":2,"color":"red" }] }}'
    ]
    data_blue = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":2,"color":"blue" }] }}'
    ]
    data_green = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":2,"color":"green" }] }}'
    ]
    data_yellow = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":2,"color":"yellow" }] }}'
    ]
    data_purple = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":2,"color":"purple" }] }}'
    ]
    data_none = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":2,"color":"none" }] }}'
    ]
    print("led2 red on")
    send_data_to_hid(vendor_id, product_id, data_red)
    time.sleep(2)
    print("led2 green on")
    send_data_to_hid(vendor_id, product_id, data_green)
    time.sleep(2)
    print("led2 blue on")
    send_data_to_hid(vendor_id, product_id, data_blue)
    time.sleep(2)
    print("led2 yellow on")
    send_data_to_hid(vendor_id, product_id, data_yellow)
    time.sleep(2)
    print("led2 purple on")
    send_data_to_hid(vendor_id, product_id, data_purple)
    time.sleep(2)
    print("led2 all off")
    send_data_to_hid(vendor_id, product_id, data_none)
    time.sleep(2)

    data_red = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":3,"color":"red" }] }}'
    ]
    data_blue = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":3,"color":"blue" }] }}'
    ]
    data_green = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":3,"color":"green" }] }}'
    ]
    data_yellow = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":3,"color":"yellow" }] }}'
    ]
    data_purple = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":3,"color":"purple" }] }}'
    ]
    data_none = [
        '{"msgId": 12345, "methodName": "ledCtl", "payload": { "ledList":',
        '[{ "ledIndex":3,"color":"none" }] }}'
    ]
    print("led3 red on")
    send_data_to_hid(vendor_id, product_id, data_red)
    time.sleep(2)
    print("led3 green on")
    send_data_to_hid(vendor_id, product_id, data_green)
    time.sleep(2)
    print("led3 blue on")
    send_data_to_hid(vendor_id, product_id, data_blue)
    time.sleep(2)
    print("led3 yellow on")
    send_data_to_hid(vendor_id, product_id, data_yellow)
    time.sleep(2)
    print("led3 purple on")
    send_data_to_hid(vendor_id, product_id, data_purple)
    time.sleep(2)
    print("led3 all off")
    send_data_to_hid(vendor_id, product_id, data_none)
    time.sleep(2)

