from matplotlib import pyplot as plt
from matplotlib import animation
import math

G = 6.6742 * 10 ** -11

SUN_MASS = 1.989 * 10 ** 6
SUN_RADIUS = 696340 * 10 ** 3
EARTH_MASS = 5.9726 * 10 ** 1
EARTH_RADIUS = 6371 * 10 ** 3

# steglängd
dt = 10

# intierar plotten samt figurerna
fig = plt.figure()
fig.set_size_inches(7, 6.5)

ax = plt.axes(xlim=(-30, 50), ylim=(-30, 50))
sun = plt.Circle((15, 10), 2, fc='y')
earth = plt.Circle((15.0001, 5), 1, fc='g')


def get_r_hat(r1, r2):
    """ Ger riktning på kraften """
    return (r1-r2)/abs(r1-r2)


def get_dist(pos1, pos2):
    """ Ger distansen mellan två positioner med pythagoras """
    return math.sqrt((pos1[0] - pos2[0]) ** 2 + (pos1[1] - pos2[1]) ** 2)


def get_alpha(pos1, pos2):
    """ Ger vinkeln mellan två positioner """
    return math.atan2((pos2[0] - pos1[0]), (pos2[1] - pos1[1]))


def init():
    """ Sätter startposition på kropparna """
    sun.center = (15, 10)
    earth.center = (15.0001, 5)
    ax.add_patch(sun)
    ax.add_patch(earth)
    return sun, earth


def animate(i):
    """
    Animeringsfunktion som räknar ut krafter, accelerationer och hastigheter för kropparna
    i både x- och y-led
    """

    sun_x, sun_y = sun.center
    earth_x, earth_y = earth.center

    r_hat_x = get_r_hat(earth_x, sun_x)
    r_hat_y = get_r_hat(earth_y, sun_y)
    alpha = get_alpha((sun_x, suny), (earth_x, earth_y))

    dist_between = get_dist((sunx, suny), (earthx, earthy))

    # x-komponenten för gravitationskraften
    force_x = - G * SUN_MASS * EARTH_MASS * r_hat_x / (dist_between ** 2) * math.cos(alpha)

    # y-komponenten för gravitationskraften
    force_y = - G * SUN_MASS * EARTH_MASS * r_hat_y / (dist_between ** 2) * math.sin(alpha)

    # x och y-komponenten av accelerationen till solen och jorden
    ax_earth = force_x/EARTH_MASS
    ay_earth = force_y/EARTH_MASS

    ax_sun = force_x/SUN_MASS
    ay_sun = force_y/SUN_MASS

    # jordens initialhastigheter i x- och y-led
    earth_x = -0.01
    earth_y = 0.008

    # solens initialhastigheter i x- och y-led. Kan sättas till andra värden
    sun_x = 0
    sun_y = 0
    if i > 0:
        # x och y-komponenten av hastigheten till solen och jorden
        earth_x = (earth_x + ax_earth * dt) * math.cos(alpha)
        earth_y = (earth_y + ay_earth * dt) * math.sin(alpha)
        sun_x = (sun_x + ax_sun * dt) * math.cos(alpha)
        sun_y = (sun_y + ay_sun * dt) * math.sin(alpha)

    # nya x- och y-positioner till solen och jorden
    x_earth = earth_x + earth_x * dt
    y_earth = earth_y + earth_y * dt
    x_sun = sunx + sun_x*dt
    y_sun = suny + sun_y*dt

    earth.set_center((x_earth, y_earth))
    sun.set_center((x_sun, y_sun))

    return sun, earth


anim = animation.FuncAnimation(fig, animate,
                               init_func=init,
                               frames=2500,
                               interval=10,
                               blit=True)
anim.save('solarsystemv1.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
plt.show()
