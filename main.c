#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xdamage.h>


int main(int argc, char *argv[])
{
    printf("Hello World!\n");

    Display *disp = XOpenDisplay(NULL);
    printf("XOpenDisplay() result: %p\n", disp);
    if (!disp) {
        return EXIT_FAILURE;
    }

    int xdevent_base = 0, xderror_base = 0;
    XDamageQueryExtension(disp, &xdevent_base, &xderror_base);

    printf("XDamageQueryExtension() result: event: %d, error: %d\n",
           xdevent_base, xderror_base);

    int major_ver = DAMAGE_MAJOR;
    int minor_ver = DAMAGE_MINOR;
    Status ver_check = XDamageQueryVersion(disp, &major_ver, &minor_ver);
    printf("XDamageQueryVersion() result: status = %d, version = %d.%d\n",
           ver_check, major_ver, minor_ver);


    Drawable drawable = DefaultRootWindow(disp);
    printf("drawable = %lu\n", drawable);

    Damage dmg = XDamageCreate(disp, drawable, XDamageReportRawRectangles);
    //Damage dmg = XDamageCreate(disp, drawable, XDamageReportDeltaRectangles);
    //Damage dmg = XDamageCreate(disp, drawable, XDamageReportNonEmpty);
    printf("XDamageCreate() result = %lu\n", dmg);
    if (!dmg) {
        fprintf(stderr, "Failed to create XDamage notification handle!\n");
        XCloseDisplay(disp);
        return EXIT_FAILURE;
    }

    printf("Entering event loop...\n");
    XEvent event;
    //while (XPending(disp)) {
    while(1) {
        XNextEvent(disp, &event);
        /* ...handle the event */
        printf(" * event type = %d\n", event.type);

        if ((event.type >= xdevent_base) &&
                (event.type <= (xdevent_base + XDamageNumberEvents))) {
            XDamageNotifyEvent *xdmgevt = (XDamageNotifyEvent *)&event;
            printf("    this is damage event...\n");
            printf("    damage = %lu\n", xdmgevt->damage);
            printf("    level = %d\n", xdmgevt->level);
            printf("    more events = %d\n", xdmgevt->more);
            printf("    timestamp = %lu\n", xdmgevt->timestamp);
            printf("    area = (%d, %d) size (%d, %d)\n",
                   (int)xdmgevt->area.x,     (int)xdmgevt->area.y,
                   (int)xdmgevt->area.width, (int)xdmgevt->area.height);
            printf("    geometry = (%d, %d) size (%d, %d)\n",
                   (int)xdmgevt->geometry.x,     (int)xdmgevt->geometry.y,
                   (int)xdmgevt->geometry.width, (int)xdmgevt->geometry.height);

            XDamageSubtract(disp, xdmgevt->damage, None, None);
        }

        if ((event.type >= xderror_base) &&
                (event.type < (xderror_base + XDamageNumberErrors))) {
            printf("    ERROR: XDamage error event!\n");
        }
    }

    //while (XCheckTypedEvent(QX11Info::display(), d->xdamageBaseEvent + XDamageNotify, &ev)) {
    //    handleXDamage(&ev);
    //}
    //XDamageSubtract(QX11Info::display(), d->damage, None, None);

    printf("Event loop ended. Will now cleanup...\n");
    XDamageDestroy(disp, dmg);
    XCloseDisplay(disp);
    return 0;
}
