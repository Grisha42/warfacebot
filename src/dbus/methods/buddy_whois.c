/**
 * WarfaceBot, a blind XMPP client for Warface (FPS)
 * Copyright (C) 2015-2017 Levak Borok <levak92@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gio/gio.h>

#include <wb_dbus_methods.h>

#include <wb_cmd.h>


struct cb_args
{
    Warfacebot *object;
    GDBusMethodInvocation *invocation;
};

void whois_cb(const struct cmd_whois_data *whois,
              void *args)
{
    struct cb_args *a = (struct cb_args *) args;

    GVariantBuilder builder;
    GVariant *result;

    g_variant_builder_init(&builder, G_VARIANT_TYPE("a{sv}"));

    if (whois != NULL)
    {
        g_variant_builder_add(&builder, "{sv}", "ip", g_variant_new_string(whois->ip ? whois->ip : ""));
        g_variant_builder_add(&builder, "{sv}", "country", g_variant_new_string(whois->country ? whois->country : ""));
        g_variant_builder_add(&builder, "{sv}", "status", g_variant_new_string(whois->status));
        g_variant_builder_add(&builder, "{sv}", "profile_id", g_variant_new_string(whois->profile_id));
        g_variant_builder_add(&builder, "{sv}", "online_id", g_variant_new_string(whois->online_id));
        g_variant_builder_add(&builder, "{sv}", "login_time", g_variant_new_int32(whois->login_time));
        g_variant_builder_add(&builder, "{sv}", "rank", g_variant_new_int32(whois->rank));
    }
    else
    {
        g_variant_builder_add(&builder, "{sv}", "error", g_variant_new_int32(1));
    }

    result = g_variant_builder_end(&builder);

    warfacebot_complete_buddy_whois(
        a->object,
        a->invocation,
        result);

    g_free(a);
}

/*
** DBus method call: "BuddyWhois"
*/
gboolean on_handle_buddy_whois(Warfacebot *object,
                               GDBusMethodInvocation *invocation,
                               const gchar *arg_Nickname)
{
    struct cb_args *a = g_new0(struct cb_args, 1);

    a->object = object;
    a->invocation = invocation;

    cmd_whois(arg_Nickname, whois_cb, a);

    return TRUE;
}
