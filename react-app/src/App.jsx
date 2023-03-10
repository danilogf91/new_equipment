// import "./App.css";
import ContainerCards from "./components/ContainerCards";
import TemperaturaHumedad from "./components/TemperaturaHumedad";
import Title from "./components/Title";

function App() {
  // const [count, setCount] = useState(0);

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

export default App;
