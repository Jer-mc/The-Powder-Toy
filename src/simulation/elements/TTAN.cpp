#include "simulation/ElementCommon.h"
#include "simulation/Air.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_TTAN()
{
	Identifier = "DEFAULT_PT_TTAN";
	Name = "TTAN";
	Colour = PIXPACK(0x909090);
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 170;

	Weight = 300;

	HeatConduct = 251;
	Description = "Titanium. High melting temperature, blocks all air pressure.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_HOT_GLOW|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1941.0f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	int ttan = 0;
	if (nt <= 2)
		ttan = 2;
	else if (parts[i].tmp)
		ttan = 2;
	else if (nt <= 6)
	{
		for (int rx = -1; rx <= 1; rx++)
			for (int ry = -1; ry <= 1; ry++)
				if ((!rx != !ry) && BOUNDS_CHECK)
				{
					if (TYP(pmap[y+ry][x+rx]) == PT_TTAN)
						ttan++;
				}
	}

	if (ttan >= 2)
	{
		sim->air->bmap_blockair[y/CELL][x/CELL] = 1;
		sim->air->bmap_blockairh[y/CELL][x/CELL] = 0x8;
	}

	if ((parts[i].temp > 1940)) {
		int r, rx, ry;
		float cxy = 0;
		for (rx = -2; rx < 3; rx++)
			for (ry = -2; ry < 3; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y + ry][x + rx];
					if (!r)
						continue;
					if (TYP(r) == PT_STEL)
					{
						if (RNG::Ref().chance(1, 1))
						{
							sim->create_part(i, x, y, PT_TTSL);
							sim->kill_part(ID(r));

						}
					}
				}
	}
	return 0;
}
