using Plugin.BLE;
using Plugin.BLE.Abstractions;
using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.Extensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices.ObjectiveC;
using System.Threading.Tasks;

namespace BeyondBatteries
{
    public partial class MainPage : ContentPage
    {
        private readonly IAdapter _bluetoothAdapter;
        IDevice _connectedDevice;
        String action;
        public MainPage()
        {
            InitializeComponent();
            _bluetoothAdapter = CrossBluetoothLE.Current.Adapter;

            _bluetoothAdapter.DeviceDiscovered += async (sender, foundBleDevice) =>
            {
                 
                if (foundBleDevice != null && !string.IsNullOrEmpty(foundBleDevice.Device.Name))
                {
                    if (!foundBleDevice.Device.Name.StartsWith("Beyond-Battery"))
                    {
                        return;
                    }
                    IsBusyIndicator.IsVisible = IsBusyIndicator.IsRunning = !(BLEConnect.IsEnabled = true);
                    await _bluetoothAdapter.StopScanningForDevicesAsync();
                    action = await DisplayActionSheet("Choose device", "Cancel", null, foundBleDevice.Device.Name);

                    if (action == foundBleDevice.Device.Name)
                    {
                        
                        _connectedDevice = foundBleDevice.Device;
                        if (_connectedDevice.State == DeviceState.Connected)
                        {
                            await Navigation.PushAsync(new DevicePage(foundBleDevice.Device));
                        }
                        else
                        {
                            
                            var connectParameters = new ConnectParameters(false, true);
                            await _bluetoothAdapter.ConnectToDeviceAsync(_connectedDevice, connectParameters);

                            await Navigation.PushAsync(new DevicePage(_connectedDevice));
                        }

                    }
                }
            
            };
        }

        private async Task<bool> PermissionGrantedAsync()
        {

            if (Microsoft.Maui.Devices.DeviceInfo.Version.Major >= 12)
            {

                PermissionStatus status = await Permissions.CheckStatusAsync<MyBluetoothPermission>();

                if (status != PermissionStatus.Granted)
                {
                    status = await Permissions.RequestAsync<MyBluetoothPermission>();
                }
                else
                {
                    return status == PermissionStatus.Granted;
                }

                if (Permissions.ShouldShowRationale<MyBluetoothPermission>())
                {
                    await Shell.Current.DisplayAlert("Needs permissions", "Location permission is required for bluetooth scanning. " +
                        "The location will not be stored or used", "Ok");
                    status = await Permissions.RequestAsync<MyBluetoothPermission>();
                }
                return status == PermissionStatus.Granted;

            }
            else
            {
                PermissionStatus status = await Permissions.CheckStatusAsync<Permissions.LocationWhenInUse>();
                if (status != PermissionStatus.Granted)
                {
                    status = await Permissions.RequestAsync<Permissions.LocationWhenInUse>();
                }
                return status == PermissionStatus.Granted;
            }
        }

        private async void OnPress(object sender, EventArgs e)
        {
            IsBusyIndicator.IsVisible = IsBusyIndicator.IsRunning = !(BLEConnect.IsEnabled = false);
            if (!await PermissionGrantedAsync())                                                           // Make sure there is permission to use Bluetooth
            {
                await DisplayAlert("Permission required", "Application needs location permission." + "Permissions needs to be set manually!", "OK");
                return;
            }


            if(!_bluetoothAdapter.IsScanning)
            {
                await _bluetoothAdapter.StartScanningForDevicesAsync();
            }
            
            IsBusyIndicator.IsVisible = IsBusyIndicator.IsRunning = !(BLEConnect.IsEnabled = true);

        }



    }
}