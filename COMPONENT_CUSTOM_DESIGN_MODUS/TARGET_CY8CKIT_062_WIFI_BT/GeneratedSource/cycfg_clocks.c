/*******************************************************************************
* File Name: cycfg_clocks.c
*
* Description:
* Clock configuration
* This file was automatically generated and should not be modified.
* Tools Package 2.1.0.1266
* psoc6pdl 1.4.0.1889
* personalities 1.0.0.0
* udd 1.1.3.51
*
********************************************************************************
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#include "cycfg_clocks.h"

#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_COMM_CLK_DIV_obj = 
	{
		.type = CYHAL_RSC_CLOCK,
		.block_num = CYBSP_CSD_COMM_CLK_DIV_HW,
		.channel_num = CYBSP_CSD_COMM_CLK_DIV_NUM,
	};
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_CSD_CLK_DIV_obj = 
	{
		.type = CYHAL_RSC_CLOCK,
		.block_num = CYBSP_CSD_CLK_DIV_HW,
		.channel_num = CYBSP_CSD_CLK_DIV_NUM,
	};
#endif //defined (CY_USING_HAL)


void init_cycfg_clocks(void)
{
	Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 1U);
	Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 1U, 6U);
	Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 1U);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_COMM_CLK_DIV_obj);
#endif //defined (CY_USING_HAL)

	Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 3U);
	Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 3U, 255U);
	Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 3U);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_CSD_CLK_DIV_obj);
#endif //defined (CY_USING_HAL)
}
