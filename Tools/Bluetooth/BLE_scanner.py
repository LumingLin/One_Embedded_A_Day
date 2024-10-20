import asyncio
from bleak import BleakScanner

async def scan_devices():
    print("Scanning for nearby Bluetooth devices...")
    
    # Discover devices with a timeout of 5 seconds
    devices = await BleakScanner.discover(timeout=5.0)
    
    print(f"Found {len(devices)} devices:")
    for i, device in enumerate(devices, 1):
        print(f"{i}. Address: {device.address}, Name: {device.name or 'Unknown'}")
        
        # Print additional details if available
        if device.metadata:
            print(f"   RSSI: {device.rssi} dBm")
            print(f"   Manufacturer Data: {device.metadata.get('manufacturer_data', 'N/A')}")
            print(f"   Service Data: {device.metadata.get('service_data', 'N/A')}")
    
    return devices

async def main():
    try:
        await scan_devices()
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    asyncio.run(main())
