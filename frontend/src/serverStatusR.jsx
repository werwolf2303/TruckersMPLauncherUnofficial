import ServerModulesContainer from "./serverStatus/serverModulesContainer";


function Root() {
    return (
        <div>
            <a id={"etsServerHeader"}>Euro Truck Simulator 2</a>
            <ServerModulesContainer game={"ETS2"}></ServerModulesContainer>
            <a id={"atsServerHeader"}>American Truck Simulator</a>
            <ServerModulesContainer game={"ATS"}></ServerModulesContainer>
        </div>
    );
}

const root = ReactDOM.createRoot(document.getElementById("content"))
root.render(Root())