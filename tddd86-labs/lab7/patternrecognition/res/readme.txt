/**********************************************************************
 *  M�nsterigenk�nning readme.txt
 **********************************************************************/

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga k�rtider i sekunder
 *  analys      n�r det k�nns vettigt att v�nta p� hela ber�kningen.
 *              Ge uppskattningar av k�rtiden i �vriga fall.
 *
 **********************************************************************/

      N       brute       sortering
 ----------------------------------
    150       0.099       0.078
    200       0.181       0.108
    300       0.515       0.265
    400       1.188       0.387
    800       9.266       0.767
   1600       73.596      3.015
   3200       588.768     11.671
   6400       4710144     52.942
  12800       37681152    228.588


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck f�r v�rstafallstiden f�r programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(N⁴) pga de fyra for-looparna, då worst case är när tre första punkterna
ligger på samma linje, och man då måste betrakta den fjärde punkten. Average blir
O(n³) eftersom du kommer minst behöva gå igenom de tre första looparna.

Sortering: O(log(N)*N²) pga två nästlade for-loopar, där vi först kollar på varje punkt p,
 och sedan jämför den med resterande punkter framför p i vektorn. I den andra for-loopen så 
 slår vi upp i mapen vilket har O(log(N)) i tidskomplexitet. I den tredje for-loopen
 går igenom sortingMaps värden, vilket kommer att vara 1 när vi har värsta fallet
 vilket gör att värstafallstiden blir O(log(N)*N²).
