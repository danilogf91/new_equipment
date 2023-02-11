import React from "react";
import CardOutput from "./CardOutput";
import classes from "./ContainerCards.module.css";
import Card from "./Card";

const ContainerCards = () => {
  const data = ["Motor 1", "Motor 2", "Motor 3", "Motor 4"];
  const dataObj = [
    { Out: "Motor 1", Pin: "GPIO1", Status: true },
    { Out: "Motor 2", Pin: "GPIO2", Status: false },
    { Out: "Motor 3", Pin: "GPIO3", Status: true },
    { Out: "Motor 4", Pin: "GPIO4", Status: false },
  ];
  return (
    <div className={classes.containerCard}>
      {/* {data.map((element) => (
        <CardOutput key={element} ledTitle={element} name={element} />
      ))} */}
      {dataObj.map(({ Out, Pin, Status }) => (
        <CardOutput key={Out} ledTitle={Pin} name={Out} status={Status} />
      ))}
      <Card />
    </div>
  );
};

export default ContainerCards;
