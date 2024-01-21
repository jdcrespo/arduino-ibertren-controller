import React, { useState } from "react";
import { ConfigProvider, Button, Progress, Layout, Space } from "antd";
import { theme } from "./common/theme";
import { AppContainer } from "./common/styles";

interface AppState {
  serialPort: SerialPort | null;
  readStream: WritableStream | null;
  currentThrottle: number;
}

function App() {
  const [state, setState] = useState<AppState>({
    serialPort: null,
    readStream: null,
    currentThrottle: 0,
  });

  const handleConnect = async () => {
    if (state.serialPort) {
      await state.serialPort.readable.cancel();
      await state.serialPort.close();
      await state.readStream?.close();
      setState((prevState) => ({
        ...prevState,
        serialPort: null,
        readStream: null,
      }));
    }

    const port = await navigator.serial.requestPort();

    await port.open({ baudRate: 115200 });

    const appendStream = new WritableStream({
      write(chunk) {
        const newThrottle = parseInt(`${chunk}`.replace("THROTLE-1=", ""));
        if (!isNaN(newThrottle)) {
          setState((prevState) => ({
            ...prevState,
            currentThrottle: newThrottle,
          }));
        }
      },
    });
    setState((prevState) => ({
      ...prevState,
      serialPort: port,
      readStream: appendStream,
    }));

    port.readable.pipeThrough(new TextDecoderStream()).pipeTo(appendStream);
  };

  return (
    <ConfigProvider theme={theme}>
      <AppContainer>
        <Space direction="vertical" style={{ width: "100%" }}>
          <Button onClick={handleConnect}>
            {state.serialPort ? "Disconnect" : "Connect"}
          </Button>
          <Progress
            type="dashboard"
            percent={Math.round((state.currentThrottle * 99) / 1023)}
          />
        </Space>
      </AppContainer>
    </ConfigProvider>
  );
}

export default App;
