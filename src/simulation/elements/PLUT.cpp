#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_PLUT()
{
	Identifier = "DEFAULT_PT_PLUT";
	Name = "PLUT";
	Colour = PIXPACK(0x407020);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.4f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x001FCE00;

	Weight = 85;

	DefaultProperties.temp = R_TEMP + 4.0f + 273.15f;
	HeatConduct = 251;
	Description = "Plutonium. Heavy, fissile particles. Generates neutrons under pressure.";

	Properties = TYPE_PART|PROP_NEUTPASS|PROP_RADIOACTIVE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (RNG::Ref().chance(1, 100) && RNG::Ref().chance(5.0f*sim->pv[y/CELL][x/CELL], 1000))
	{
		sim->create_part(i, x, y, PT_NEUT);
	}

	if ((parts[i].temp > 1200)) {
		int r, rx, ry;
		float cxy = 0;
		for (rx = -2; rx < 3; rx++)
			for (ry = -2; ry < 3; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y + ry][x + rx];
					if (!r)
						continue;
					if (TYP(r) == PT_URAN)
					{
						if (RNG::Ref().chance(99, 100))
						{
							sim->create_part(i, x, y, PT_PLTU);
							sim->kill_part(ID(r));

						}
					}
				}
	}

	return 0;
}
