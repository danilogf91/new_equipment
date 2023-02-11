import "./app.css";
import Title from "./components/Title";
import TemperaturaHumedad from "./components/TemperaturaHumedad";
import ContainerCards from "./components/ContainerCards";

export function App() {
  const data = {
    Led1: "Motor 1",
    Led2: "Motor 2",
    Led3: "Motor 3",
    Led4: "Motor 4",
  };

  return (
    <>
      <Title title="Control equipment" />
      <TemperaturaHumedad />
      <ContainerCards data={data} />
    </>
  );
}
