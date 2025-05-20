import javax.swing.*;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class RouterNode {
    private int myID;
    private GuiTextArea myGUI;
    private RouterSimulator sim;
    private int[] costs;
    private int[][] distvector = new int[RouterSimulator.NUM_NODES][RouterSimulator.NUM_NODES];
    private Map<Integer, Integer> pathMap = new HashMap<>();

    //--------------------------------------------------
    public RouterNode(int ID, RouterSimulator sim, int[] costs) {
        myID = ID;
        this.sim = sim;
        this.costs = new int[RouterSimulator.NUM_NODES];
        myGUI = new GuiTextArea("  Output window for Router #" + ID + "  ");
        System.arraycopy(costs, 0, this.costs, 0, RouterSimulator.NUM_NODES);

        //Fyller distvector med infinity som startvärde
        for (int[] i : distvector) {
            Arrays.fill(i, RouterSimulator.INFINITY);
        }

        for (int node = 0; node < costs.length; ++node) {
            distvector[ID][node] = costs[node];

            if (node != ID && costs[ID] != RouterSimulator.INFINITY) {
                pathMap.put(node, node);
            }
        }
        //distans till själv = 0
        distvector[ID][ID] = 0;
        update();
    }

    //--------------------------------------------------
    public void recvUpdate(RouterPacket pkt) {
        distvector[pkt.sourceid] = pkt.mincost.clone();
        shortestPath();
    }


    //--------------------------------------------------
    private void sendUpdate(RouterPacket pkt) {
        sim.toLayer2(pkt);
    }

    public void shortestPath() {
        boolean hasChanged = false;
        for (int node = 0; node < distvector[myID].length; ++node) {
            int cheapest = costs[node];
            int passingNode = node;
            for (int neigh = 0; neigh < costs.length; ++neigh) {
                if (costs[neigh] != RouterSimulator.INFINITY && neigh != myID) {
                    int neighcost = costs[neigh] + distvector[neigh][node];
                    if (neighcost < cheapest) {
                        cheapest = neighcost;
                        passingNode = neigh;
                    }
                }
            }
            if (cheapest != distvector[myID][node]) {
                distvector[myID][node] = cheapest;
                pathMap.put(node, passingNode);
                hasChanged = true;
            }

        }
        if (hasChanged) {
            update();
        }

    }

    public void update() {
        for (Map.Entry<Integer, Integer> entry : pathMap.entrySet()) {
            int[] distCopy = distvector[myID].clone();
            if (RouterSimulator.POISONREVERSE) {
                myGUI.print("POSIONREVERSE");
                for (Map.Entry<Integer, Integer> entry2 : pathMap.entrySet()) {
                    if (entry.getKey() != entry2.getKey() && pathMap.get(entry.getKey()) == entry.getKey()) {
                        distCopy[entry.getKey()] = RouterSimulator.INFINITY;
                    }
                }
            }
            sendUpdate(new RouterPacket(myID, entry.getKey(), distCopy));
        }
    }

    //--------------------------------------------------
    public void printDistanceTable() {
        myGUI.println("Current table for " + myID +
                "  at time " + sim.getClocktime() + "\n");
        printNeighbourTable();
        printDistVector();
    }

    public void printNeighbourTable() {
        myGUI.println("Distancetable: ");
        printFirstLine();
        for (int neigh = 0; neigh < costs.length; neigh++) {
            myGUI.print(" nbr " + neigh + " |");
            for (int d : distvector[neigh]) {
                myGUI.print(String.format("%10d", d));
            }
            myGUI.println();
        }
    }

    public void printDistVector() {
        myGUI.println("\n");
        myGUI.println("Our distance vector and routes: ");
        printFirstLine();

        myGUI.print(String.format("%9s", "cost |"));

        for (int cost : distvector[myID]) {
            myGUI.print(String.format("%10d", cost));
        }
        myGUI.println();
        myGUI.print(String.format("%8s", "route |"));
        for (int node = 0; node < costs.length; ++node) {
            String nextNode = "-";
            if (costs[node] != RouterSimulator.INFINITY) {
                if (myID == node) {
                    nextNode = Integer.toString(myID);
                } else {
                    nextNode = Integer.toString(pathMap.get(node));
                }
            }
            myGUI.print(String.format("%10s", nextNode));
        }
        myGUI.println();
    }


    public void printFirstLine() {
        myGUI.print(String.format("%10s", "dst |"));
        StringBuilder divider = new StringBuilder("-----------");

        for (int nodeID = 0; nodeID < RouterSimulator.NUM_NODES; nodeID++) {
            myGUI.print(String.format("%8d", nodeID));
            divider.append("---------");
        }

        myGUI.println();
        myGUI.println(divider.toString());
    }


    //--------------------------------------------------
    public void updateLinkCost(int dest, int newcost) {
        costs[dest] = newcost;
        shortestPath();
    }

}
