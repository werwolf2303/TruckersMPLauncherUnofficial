export default function ServerModulesContainer({game}) {
    const [loaded, setLoaded] = React.useState(false);
    const [errorState, setErrorState] = React.useState(0);
    const [serversETS2, setServersETS2] = React.useState([]);
    const [serversATS, setServersATS] = React.useState([]);
    const [id, setId] = React.useState("");

    class Server {
        game
        ip
        port
        name
        online
        players
        maxplayers
        mapid
        displayorder
        speedlimiter
        collisions
        carsforplayers
        policecarsforplayers
        afkenabled
        event
        specialEvent
        promods
    }

    React.useEffect(() => {
        const asyncLoad = async () => {
            const getServersReq = new XMLHttpRequest();
            getServersReq.onload = function () {
                try {
                    var serverJSON = JSON.parse(getServersReq.responseText);
                    var svsETS2 = [];
                    var svsATS = [];
                    for (let i = 0; i < serverJSON["response"].length; i++) {
                        let server = serverJSON["response"][i];
                        var sobj = new Server();
                        sobj.game = server["game"];
                        sobj.ip = server["ip"];
                        sobj.port = server["port"];
                        sobj.name = server["name"];
                        sobj.online = server["online"];
                        sobj.players = server["players"];
                        sobj.maxplayers = server["maxplayers"];
                        sobj.mapid = server["mapid"];
                        sobj.displayorder = server["displayorder"];
                        sobj.speedlimiter = server["speedlimiter"];
                        sobj.collisions = server["collisions"];
                        sobj.carsforplayers = server["carsforplayers"];
                        sobj.policecarsforplayers = server["policecarsforplayers"];
                        sobj.afkenabled = server["afkenabled"];
                        sobj.event = server["event"];
                        sobj.specialEvent = server["specialEvent"];
                        sobj.promods = server["promods"];
                        if (sobj.game === "ETS2") {
                            svsETS2.push(sobj);
                        }
                        if (sobj.game === "ATS") {
                            svsATS.push(sobj);
                        }
                    }
                    setServersETS2(svsETS2);
                    setServersATS(svsATS);
                } catch (ex) {
                    setErrorState(1);
                }
                setLoaded(true);
            }
            getServersReq.onerror = function () {
                setErrorState(1);
                setLoaded(true);
            }
            getServersReq.open("GET", "bridge/getServers");
            getServersReq.send();
        }
        asyncLoad();

        if(game === "ATS") setId("atsserverModulesContainer");
        if(game === "ETS2") setId("ets2serverModulesContainer");
    }, []);

    return (
        <div id={id}>
            {!loaded &&
                <a>Loading...</a>
            }
            {errorState === 1 &&
                <a>An error occurred</a>
            }
            {loaded && errorState === 0 && game==="ETS2" && serversETS2.map((server, index) => (
                <div id={"server" + index}>
                    <a id={"name"}>{server.name} </a>
                    <a id={"playerStats"}>{server.players} / {server.maxplayers}</a>
                    <button onClick={() => {
                        document.getElementById("server" + index).getElementsByTagName("button")[0].classList.toggle("active");
                        var content = document.getElementById("server" + index).getElementsByTagName("div")[0];
                        if (content.style.display === "block") {
                            content.style.display = "none";
                        } else {
                            content.style.display = "block";
                        }
                    }} id={"collapseButton"} type={"button"}><img id={"collapseButtonIcon"}/></button>
                    <div id={"collapseContainer"}>
                        <div id={"speedLimiterContainer"}>
                            <img id={"speedLimiterIcon"}/>
                            <a>Speed Limiter</a>
                            <img id={"speedLimiterCheckmark"}/>
                        </div>
                        <div id={"automaticAfkKickContainer"}>
                            <img id={"automaticafkkickIcon"}/>
                            <a>Automatic AFK Kick</a>
                            <img id={"automaticafkkickCheckmark"}/>
                        </div>
                        <div id={"collisionsContainer"}>
                            <img id={"collisionsIcon"}/>
                            <a>Collisions</a>
                            <img id={"collisionsCheckmark"}/>
                        </div>
                        <div id={"carsforplayersContainer"}>
                            <img id={"carsforplayersIcon"}/>
                            <a>Cars for Players</a>
                            <img id={"carsforplayersCheckmark"}/>
                        </div>
                        <div id={"eventContainer"}>
                            <img id={"eventIcon"}/>
                            <a>Event</a>
                            <img id={"eventCheckmark"}/>
                        </div>
                        <div id={"policecarsforplayersContainer"}>
                            <img id={"policecarsforplayersIcon"}/>
                            <a>Police Cars for Players</a>
                            <img id={"policecarsforplayersCheckmark"}/>
                        </div>
                    </div>
                </div>
            ))}
            {loaded && errorState === 0 && game==="ATS" && serversATS.map((server, index) => (
                <div id={"server" + index}>
                    <a id={"name"}>{server.name} </a>
                    <a id={"playerStats"}>{server.players} / {server.maxplayers}</a>
                    <button onClick={() => {
                        document.getElementById("server" + index).getElementsByTagName("button")[0].classList.toggle("active");
                        var content = document.getElementById("server" + index).getElementsByTagName("div")[0];
                        if (content.style.display === "block") {
                            content.style.display = "none";
                        } else {
                            content.style.display = "block";
                        }
                    }} id={"collapseButton"} type={"button"}><img id={"collapseButtonIcon"}/></button>
                    <div id={"collapseContainer"}>
                        <div id={"speedLimiterContainer"}>
                            <img id={"speedLimiterIcon"}/>
                            <a>Speed Limiter</a>
                            <img id={"speedLimiterCheckmark"}/>
                        </div>
                        <div id={"automaticAfkKickContainer"}>
                            <img id={"automaticafkkickIcon"}/>
                            <a>Automatic AFK Kick</a>
                            <img id={"automaticafkkickCheckmark"}/>
                        </div>
                        <div id={"collisionsContainer"}>
                            <img id={"collisionsIcon"}/>
                            <a>Collisions</a>
                            <img id={"collisionsCheckmark"}/>
                        </div>
                        <div id={"carsforplayersContainer"}>
                            <img id={"carsforplayersIcon"}/>
                            <a>Cars for Players</a>
                            <img id={"carsforplayersCheckmark"}/>
                        </div>
                        <div id={"eventContainer"}>
                            <img id={"eventIcon"}/>
                            <a>Event</a>
                            <img id={"eventCheckmark"}/>
                        </div>
                        <div id={"policecarsforplayersContainer"}>
                            <img id={"policecarsforplayersIcon"}/>
                            <a>Police Cars for Players</a>
                            <img id={"policecarsforplayersCheckmark"}/>
                        </div>
                    </div>
                </div>
            ))}
        </div>
    );
}