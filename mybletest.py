import asyncio
from bleak import BleakClient

address = "d7:0b:cd:43:8c:fe"
MODEL_NBR_UUID = "2A24"
HR_NOTIFY_UUID = "2A37"

async def main(address):
	async with BleakClient(address) as client:
		model_number = await client.read_gatt_char(MODEL_NBR_UUID)
		print("Model Number: {0}".format("".join(map(chr, model_number))))
		async def notification_handler(characteristic, data):
			print(f"{characteristic.description}: {data}" + " | Remove the 0x06 i.e. data[0] and you get HR = " + str(int.from_bytes(data[1:], "big", signed="True")))
		await client.start_notify(HR_NOTIFY_UUID, notification_handler)
		await asyncio.sleep(30.0)
		await client.stop_notify(HR_NOTIFY_UUID)

		#write_value = bytearray([0x01])
        #value = await client.read_gatt_char(IO_DATA_CHAR_UUID)
		#print("I/O Data Pre-Write Value: {0}".format(value))

asyncio.run(main(address))
